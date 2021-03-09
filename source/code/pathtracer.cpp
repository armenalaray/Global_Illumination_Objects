
#include "pathtracer.h"

#if 0
hitable * CreateWorld(float tMin, float tMax)
{
	int Width, Height, ChannelsPPixel;
	unsigned char * TextData = stbi_load("../../source/data/images/earthmap.jpg", &Width, &Height, &ChannelsPPixel, 0);
	if(TextData)
	{
		int i = 0;
		int n = 50;
		hitable ** List = new hitable*[n];
		
		List[i++] = new sphere(vec3(0,-1000, 0), 1000, new lambertian(new noise_texture<perlin_noise>(vec3(NV255(104), NV255(168), NV255(11)))));
		List[i++] = new sphere(vec3(0,1.0f,0), 1.0f, new dielectric(1.5f));
		List[i++] = new sphere(vec3(4.0f,1.0f,0), 1.0f, new lambertian(new marble_texture<perlin_noise>(vec3(1.0f,1.0f,1.0f), 1.0f, 2.0f, 0.5f, 5)));
		List[i++] = new sphere(vec3(-4.0f,1.0f,0), 1.0f, new metal(vec3(0.7f, 0.6f, 0.5f), 0.0f));
		List[i++] = new sphere(vec3(8.0f,1.0f,0), 1.0f, new lambertian(new image_texture(TextData, Width, Height)));
		
#if 0
		List[i++] = new sphere(vec3(8.0f,1.0f, 0), 1.0f, new lambertian(new wood_texture<value_noise>(vec3(NV255(216), NV255(162), NV255(88)), 
																									  vec3(NV255(249), NV255(204), NV255(144)),
																									  2.0f, 2.0f, 0.5f, 5)));
#endif
		
		while(i < n)
		{
			for(int a = 0; a <  11 && i < n; a++)
			{
				for(int b =-11; b < 11 && i < n; b++)
				{
					float ChooseMat = RandNum0ToLessThan1();
					vec3 Center(a+0.9f*RandNum0ToLessThan1(), 0.2f, b+0.9f*RandNum0ToLessThan1());
					if((Center-vec3(4.0f,0.2f,0)).Length() > 0.9f)
					{
						if(ChooseMat < 0.8f)
						{
							// NOTE(Alex): //diffuse
							List[i++] = new moving_sphere(Center, Center + vec3(0,0.5f*RandNum0ToLessThan1(), 0), 0, 1, 0.2f, 
														  new lambertian(new constant_texture(vec3(
                                                                                                   RandNum0ToLessThan1()* RandNum0ToLessThan1(), 
                                                                                                   RandNum0ToLessThan1()* RandNum0ToLessThan1(), 
                                                                                                   RandNum0ToLessThan1()* RandNum0ToLessThan1())))); 
						}
						else if(ChooseMat < 0.95f)
						{
							// NOTE(Alex): //metal
							List[i++] = new sphere(Center, 0.2f, new metal(
                                                                           vec3(
                                                                                0.5f*(1.0f + RandNum0ToLessThan1()),
                                                                                0.5f*(1.0f + RandNum0ToLessThan1()),
                                                                                0.5f*(1.0f + RandNum0ToLessThan1())), 
                                                                           0.5f*RandNum0ToLessThan1()
                                                                           ));
							
						}
						else
						{
							// NOTE(Alex): //glass
							List[i++] = new sphere(Center, 0.2f, new dielectric(1.5f));
						}		
					}
				}
			}
		}
		
		Assert(i == n);
		return new bvh_node(List, n, tMin, tMax);
	}
	return 0;
}

#if 0
hitable * CornellSmoke()
{
	material * Red = new lambertian(new constant_texture(vec3(0.65f, 0.05f, 0.05f)));
	material * White = new lambertian(new constant_texture(vec3(0.73f, 0.73f, 0.73f)));
	material * Green = new lambertian(new constant_texture(vec3(0.12f, 0.45f, 0.15f)));
	material * Light = new diffuse_light(new constant_texture(vec3(7.f, 7.f, 7.f)));
	
	const int n = 8;
	hitable ** List = new hitable *[n];
	List[0] = new yz_rect(0,5.0f,0,5.0f,0, Green);
	List[1] = new flip_normals(new yz_rect(0,5.0f,0,5.0f,5, Red));
	List[2] = new flip_normals(new xz_rect(1,4,1,4, 4.999f, Light));
	List[3] = new xz_rect(0,5,0,5, 0, White);
	List[4] = new flip_normals(new xz_rect(0,5,0,5, 5, White));
	List[5] = new xy_rect(0,5,0,5,0, White);
	hitable * a = new rotate_y(45.0f, new rotate_x(45.0f, new rotate_z(45.0f, new rotate_z(-0.0f, new rotate_x(-0.0f, new rotate_y(-0.0f, new box(vec3(1,1,1), vec3(2,3,2),White))))))); 
	hitable * b = new box(vec3(3.4f,0,3.4f), vec3(4.2f,1,4.2f), White); 
	List[6] = new constant_medium(0.01f, a, new constant_texture(vec3(1.f, 1.f, 1.f)));
	List[7] = new constant_medium(0.01f, b, new constant_texture(vec3(0.f, 0.f, 0.f)));
	return new hitable_list(List, n);
}
#endif

hitable * Final(float Time0, float Time1)
{
	int Width, Height, ChannelsPPixel;
	unsigned char * TextData = stbi_load("../../source/data/images/earthmap.jpg", &Width, &Height, &ChannelsPPixel, 0);
	
	if(TextData)
	{
		const int n = 100;
		hitable ** List = new hitable *[n];
		
		const int BoxCountSide = 10;
		const int BoxCount = BoxCountSide * BoxCountSide * 2;
		hitable ** BoxList1 = new hitable* [BoxCount];
		material * White = new lambertian(new constant_texture(vec3(0.73f,0.73f,0.73f)));
		material * Ground = new lambertian(new constant_texture(vec3(0.48f,0.83f,0.53f)));
		material * Light = new diffuse_light(new constant_texture(vec3(15.f, 15.f, 15.f)));
		material * Wood = new lambertian(new wood_texture<value_noise>(vec3(NV255(216), NV255(162), NV255(88)), 
                                                                       vec3(NV255(249), NV255(204), NV255(144)),
																	   2.0f, 2.0f, 0.5f, 5));
		int BoxList1Index = 0;
		float BoxDim = 1.0f;
		for(int i = 0; i < BoxCountSide; i++)
		{
			for(int j = 0; j < BoxCountSide; j++)
			{
				float x0 = i * BoxDim;
				float z0 = j * BoxDim;
				float y0 = 0.0f;
				float x1 = (i + 1) * BoxDim;
				float z1 = (j + 1) * BoxDim;
				float y1 = BoxDim * RandNum0ToLessThan1() + 0.5f;
				
				//BoxList1[BoxList1Index++] = new box(vec3(x0,y0,z0), vec3(x1,y1,z1), Wood);
				
				x0 = i * BoxDim;
				z0 = j * BoxDim;
				y0 = 6.5f - BoxDim * RandNum0ToLessThan1();
				x1 = (i + 1) * BoxDim;
				z1 = (j + 1) * BoxDim;
				y1 = 7.0f;
				
				//BoxList1[BoxList1Index++] = new box(vec3(x0,y0,z0), vec3(x1,y1,z1), new metal(vec3(0.8f, 0.8f, 0.9f), 1.0f));
			}
		}
		
		int Index = 0;
		List[Index++] = new bvh_node(BoxList1, BoxCount, Time0, Time1);
		List[Index++] = new xy_rect(2,7,2,7, -3.0f, Light);
		vec3 Center(2,2,2);
		List[Index++] = new moving_sphere(Center, Center + vec3(0,1.0f,0), 0, 1, 1.f, new lambertian(new constant_texture(vec3(0.7f, 0.3f, 0.1f)
																														  )));	
		List[Index++] = new sphere(vec3(2,2,7), 1.0f, new dielectric(1.5f));
		List[Index++] = new sphere(vec3(7,2,2), 1.0f, new metal(vec3(0.8f, 0.8f, 0.9f), 10.0f));
		
		hitable * Boundary = new sphere(vec3(0,0,0), 70.0f, new dielectric(1.5));
		List[Index++] = Boundary;
		List[Index++] = new constant_medium(0.0001f, Boundary, new constant_texture(vec3(1.0f, 1.0f, 1.0f)));
		List[Index++] = new sphere(vec3(1.5f,2.0f,6.0f), 1.0f, new lambertian(new image_texture(TextData, Width, Height)));
		List[Index++] = new sphere(vec3(4.0f,4.0f,4.0f), 1.0f, new lambertian(new marble_texture<perlin_noise>(vec3(1.0f,1.0f,1.0f), 1.0f, 2.0f, 0.5f, 5)));
		
		float BoxDim2 = 1.0f;
		const int BoxList2Count = 500;
		hitable ** BoxList2 = new hitable *[BoxList2Count];
		for(int j = 0; j < BoxList2Count; j++)
		{
			BoxList2[j] = new sphere(BoxDim2 * vec3(RandNum0ToLessThan1(),RandNum0ToLessThan1(), RandNum0ToLessThan1()), 0.1f, Ground);
		}
		
		List[Index++] = new translate(vec3(10.0f, 2.0f, 9.0f), new bvh_node(BoxList2, BoxList2Count, Time0, Time1));
		
		return new hitable_list(List, Index);
	}
	return 0;
}

hitable * SimpleLight()
{
	texture * SphereTex = new marble_texture<perlin_noise>(vec3(1.0f,1.0f,1.0f), 1.0f, 2.0f, 0.5f, 5);
	texture * LightTex = new constant_texture(vec3(1,1,1));
	const int n = 6;
	hitable ** List = new hitable *[n];
	List[0] = new sphere(vec3(0, -1000, 0), 1000, new lambertian(SphereTex)); 
	List[1] = new sphere(vec3(0, 1, 0), 1, new lambertian(SphereTex)); 
	//List[2] = new sphere(vec3(0, 3, 0), 1, new diffuse_light(LightTex)); 
	List[2] = new xz_rect(-1,1,-1,1, 3, new diffuse_light(LightTex));
	List[3] = new yz_rect(0,2,-1,1,-2, new diffuse_light(LightTex));
	List[4] = new xy_rect(-1,1,0,2,-2, new diffuse_light(LightTex));
	List[5] = new xy_rect(-1,1,0,2, 2, new diffuse_light(LightTex));
	return new hitable_list(List, n);
}

hitable * OneWorld()
{
	int Width, Height, ChannelsPPixel;
	unsigned char * TextData = stbi_load("../../source/data/images/earthmap.jpg", &Width, &Height, &ChannelsPPixel, 0);
	if(TextData)
	{
		material * Mat = new lambertian(new image_texture(TextData, Width, Height));
		int n = 1;
		hitable ** List = new hitable*[n];
		List[0] = new sphere(vec3(0.0f,1,0), 1.0f, Mat);
		return new hitable_list(List, n);
	}
	return 0;
}

hitable * TwoSpheres()
{
	texture * Checker = new checker_texture(new constant_texture(vec3(0.2f,0.3f,0.1f)), new constant_texture(vec3(0.9f, 0.9f, 0.9f)));
	int n = 2;
	hitable ** List = new hitable*[n];
	List[0] = new sphere(vec3(0,-10,0), 10, new lambertian(Checker));
	List[1] = new sphere(vec3(0,10,0), 10, new lambertian(Checker));
	return new hitable_list(List, n);
}
#endif

shared_ptr<hitable> Radiosity_CornellBox(float Time0, float Time1)
{
    vector<shared_ptr<hitable>> List;
	
	shared_ptr<texture> T_XY_Z0 = make_shared<image_texture_xy_z0>("../../source/data/images/XY_Z0.png");
	shared_ptr<texture> T_XZ_Y0 = make_shared<image_texture_xz_y0>("../../source/data/images/XZ_Y0.png");
	shared_ptr<texture> T_XZ_Y5 = make_shared<image_texture_xz_y5>("../../source/data/images/XZ_Y5.png");
	shared_ptr<texture> T_YZ_X0 = make_shared<image_texture_yz_x0>("../../source/data/images/YZ_X0.png");
	shared_ptr<texture> T_YZ_X5 = make_shared<image_texture_yz_x5>("../../source/data/images/YZ_X5.png");
	
	shared_ptr<material> M_XY_Z0 = make_shared<diffuse_light>(T_XY_Z0);
	shared_ptr<material> M_XZ_Y0 = make_shared<diffuse_light>(T_XZ_Y0);
	shared_ptr<material> M_XZ_Y5 = make_shared<diffuse_light>(T_XZ_Y5);
	shared_ptr<material> M_YZ_X0 = make_shared<diffuse_light>(T_YZ_X0);
	shared_ptr<material> M_YZ_X5 = make_shared<diffuse_light>(T_YZ_X5);
    
	shared_ptr<hitable> xz1 = make_shared<xz_rect>(0.f,5.f,0.f,5.f,5.f, M_XZ_Y5);
	shared_ptr<hitable> yz1 = make_shared<yz_rect>(0.f,5.0f,0.f,5.0f,5.f, M_YZ_X5);
    
	List.push_back(make_shared<yz_rect>(0.0f,5.0f,0.0f,5.0f,0.0f, M_YZ_X0));
	List.push_back(make_shared<flip_normals>(yz1));
	
	List.push_back(make_shared<xz_rect>(0.0f,5.0f,0.0f,5.0f,0.0f, M_XZ_Y0));
    List.push_back(make_shared<flip_normals>(xz1));
    
	List.push_back(make_shared<xy_rect>(0.f,5.f,0.f,5.f,0.f, M_XY_Z0));
	
	return make_shared<hitable_list>(List);
}

shared_ptr<hitable> CornellBox(float Time0, float Time1)
{
	vector<shared_ptr<hitable>> List;
	
	shared_ptr<texture> TEarth = make_shared<image_texture>("../../source/data/images/earthmap.jpg");
	shared_ptr<texture> TRed = make_shared<constant_texture>(vec3(0.65f, 0.05f, 0.05f));
	shared_ptr<texture> TGreen = make_shared<constant_texture>(vec3(0.12f, 0.45f, 0.15f));
	shared_ptr<texture> TWhite = make_shared<constant_texture>(vec3(0.73f, 0.73f, 0.73f));
	shared_ptr<texture> TLight = make_shared<constant_texture>(vec3(15.f, 15.f, 15.f));
	shared_ptr<texture> TTriBar = make_shared<uvw_texture>(vec3(1.f, 0.f, 0.f),vec3(0.f, 1.f, 0.f),vec3(0.f, 0.f, 1.f));
	
	shared_ptr<material> MEarth = make_shared<lambertian>(TEarth);
	shared_ptr<material> Red = make_shared<lambertian>(TRed);
	shared_ptr<material> Green = make_shared<lambertian>(TGreen);
	shared_ptr<material> White = make_shared<lambertian>(TWhite);
	shared_ptr<material> Light = make_shared<diffuse_light>(TLight);
	shared_ptr<material> TriBar = make_shared<diffuse_light>(TTriBar);
	
	shared_ptr<hitable> xz0 = make_shared<xz_rect>(2.f,3.f,2.f,3.f, 4.999f, Light);
	shared_ptr<hitable> yz0 = make_shared<yz_rect>(0.f,5.0f,0.f,5.0f,5.f, Red);
	shared_ptr<hitable> xz1 = make_shared<xz_rect>(0.f,5.f,0.f,5.f,5.f, White);
	shared_ptr<hitable> Earth = make_shared<sphere>(vec3(2.0f,1.5f,2.0f), 1.5f, MEarth);
    
	List.push_back(make_shared<yz_rect>(0.0f,5.0f,0.0f,5.0f,0.0f, Green));
    
	List.push_back(make_shared<flip_normals>(xz0));
	List.push_back(make_shared<flip_normals>(yz0));
	List.push_back(make_shared<xz_rect>(0.0f,5.0f,0.0f,5.0f,0.0f, White));
	List.push_back(make_shared<xy_rect>(0.f,5.f,0.f,5.f,0.f, White));
	List.push_back(make_shared<flip_normals>(xz1));
	//List.push_back(make_shared<triangle>(vec3(2.0f, 2.0f, 2.0f),vec3(2.0f, 4.0f, 2.0f), vec3(1.0f, 2.0f, 2.0f), TriBar));	
	//List.push_back(make_shared<box>(vec3(3.4f,0,3.4f), vec3(4.2f,1,4.2f), Red));	
	
    //List.push_back(make_shared<rotate_y>(-90.0f, Earth));
	//List.push_back(make_shared<triangle_mesh>("../../source/data/models/cow.geo", TriBar));
#if 0
	vector<shared_ptr<hitable>>::pointer ptr = &List[0];
	return make_shared<bvh_node>(ptr, List.size(), Time0, Time1);
#else
	return make_shared<hitable_list>(List);
#endif
}


int main(int ArgumentCount, char ** Arguments)
{
	//cout << Arguments[0] << endl;
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	//_CrtSetBreakAlloc(147);	
	SeedInit();
	
	float AspectRatio = 16.0f / 9.0f;
    
	const int Height = 1024;
    const int Width = static_cast<int>(AspectRatio * Height); 
	
	int SamplesPerPixel = 100;
	float Time0 = 0.0f;
	float Time1 = 1.0f;
    
#if 1
	vec3 LookFrom(2.5f, 2.5f, 12.0f);
	vec3 LookAt(2.5f,2.5f,2.0f);
	vec3 VUP(0,1,0);
	float FocusDistance = (LookFrom - LookAt).Length();
	float Aperture = 0.0f;
	camera Cam(LookFrom, LookAt, VUP, 40.0f, AspectRatio, Aperture, FocusDistance, Time0, Time1);
#else
	vec3 LookFrom(2.5f, 15.0f, 20.0f);
	vec3 LookAt(2.5f,2.5f,2.0f);
	vec3 VUP(0,1,0);
	float FocusDistance = (LookFrom - LookAt).Length();
	float Aperture = 0.0f;
	camera Cam(LookFrom, LookAt, VUP, 40.0f, AspectRatio, Aperture, FocusDistance, Time0, Time1);
#endif
    
	//hitable * World = TwoSpheres();
	//hitable * World = OneWorld();
	//hitable * World = SimpleLight();	
	//hitable * World = CornellSmoke();
	//hitable * World = CreateWorld(Time0, Time1);
	//hitable * World = Final(Time0, Time1);
	//shared_ptr<hitable> World = CornellBox(Time0, Time1);
	shared_ptr<hitable> World = Radiosity_CornellBox(Time0, Time1);
	
	if(World)
	{
		vec3 MaxPixVal = vec3(FLT_MIN, FLT_MIN, FLT_MIN);
		vec3 * Buffer = new vec3[Width * Height];
		// TODO(Alex): MEMSET BUFFER, we don't happen to see garbage becuase we pass through each pixel!!
		for(int j = Height - 1; j >= 0; j--)
		{
			for(int i = 0; i < Width; i++)
			{
				vec3 Col(0,0,0);
				for(int s = 0; s < SamplesPerPixel; ++s)
				{
					float u = (float(i) + RandNum0ToLessThan1()) / float(Width);
					float v = (float(j) + RandNum0ToLessThan1()) / float(Height);
					
					ray R = Cam.GetRay(u,v);
					Col += Color(R, World, 0);
				}
				Col /= float(SamplesPerPixel);
				
                Col = vec3(sqrt(Col[0]),sqrt(Col[1]),sqrt(Col[2])); 
				Buffer[j * Width +  i] = Col;
				
				if(MaxPixVal.x < Col.x) MaxPixVal.x = Col.x;
				if(MaxPixVal.y < Col.y) MaxPixVal.y = Col.y;
				if(MaxPixVal.z < Col.z) MaxPixVal.z = Col.z;
			}
		}
		
		cout << "P3\n" << Width << " " << Height << "\n255\n";
		for(int j = Height - 1; j >= 0; j--)
		{
			for(int i = 0; i < Width; i++)
			{
				vec3 & Col = Buffer[j * Width + i];
				//Col /= MaxPixVal;
				int ir = int(255.99*Col.r);
				int ig = int(255.99*Col.g);
				int ib = int(255.99*Col.b);
				cout << ir << " " << ig << " " << ib << endl;
			}
		}
		delete [] Buffer;
    }
	return 0;
}