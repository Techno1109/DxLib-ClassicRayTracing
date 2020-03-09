#include<dxlib.h>
#include<math.h>
#include <vector>
#include <memory>
#include<omp.h>
#include<array>
#include"Geometry.h"

#include"GeometoryObject.h"
#include"Plane.h"
#include"Sphere.h"

const int screen_width = 640;
const int screen_height = 480;
const int GridSize = 30;
const Vector3 BaseLight(1, -1, -1);



float Clamp(float Param, const float Min=0.0f, const float Max=1.0f)
{
	return max(min(Param, Max), Min);
}



Vector3 GetReflectVector(const Vector3& InVec,const Vector3& NormalVec)
{
	return InVec - NormalVec*2 *  Dot(NormalVec, InVec);
}


Vector3 RotateVector(Vector3& Vec,Vector3 Axis,float Deg)
{
	Axis.Normalize();
	float Rad = Deg * (DX_PI_F / 180);
	Vector3 Center = Axis*Dot(Vec, Axis);
	Vector3 Pos = Vec - Center;
	Vector3 CrossVec = Cross(Axis, Pos);
	float Sin = std::sinf(Rad);
	float Cos = std::sinf(Rad);
	Vector3 Return= Center + (Pos * Sin) + (CrossVec * Cos);
	return Return.Normalized();
}

void
DrawPixelWithFloat(int x,int y,float r,float g, float b)
{
	DrawPixel(x, y, GetColor(r*0xff, g * 0xff, b * 0xff));
}

void
DrawPixelWithColor(int x, int y,Color& Col)
{
	DrawPixel(x, y, GetColor(Col.Red * 0xff, Col.Green * 0xff, Col.Blue * 0xff));
}

std::pair<GeometoryObject::HitInfo, Color> RecursiveTrace(std::vector<std::shared_ptr<GeometoryObject>>& TargetObjects,const Position3 & Eye, const Vector3 & Ray, const Vector3 Light, GeometoryObject* ExclusiveObject,int Count)
{
	std::pair<GeometoryObject::HitInfo, Color> ReturnPair;
	ReturnPair.first.IsHit = false;
	ReturnPair.second = Color(1, 1, 1);
		if (Count>7)
		{
			return ReturnPair;
		}

		std::vector<std::pair<GeometoryObject::HitInfo, Color>> HitDatas;
		std::pair<GeometoryObject::HitInfo, Color> ReturnData;
		auto RLight = BaseLight * -1;
		RLight.Normalize();

#pragma omp parallel for
		for (int TargetNum = 0; TargetNum < TargetObjects.size(); TargetNum++)
		{
			if (TargetObjects[TargetNum].get()==ExclusiveObject)
			{
				continue;
			}

			Vector3 TmpNormal;
			Position3 TmpHitPos;

			auto HitData = TargetObjects[TargetNum]->IsHitRayAndObject(Eye, Ray, Light, TmpNormal, TmpHitPos);
			if (HitData.first.IsHit)
			{
				if (TargetObjects[TargetNum]->GetReflectance() > 0)
				{
					//ここから下で反射処理
					Position3 ThisHitPos= TmpHitPos;
					auto RefVec = GetReflectVector(Ray, TmpNormal).Normalized();
					auto Result = RecursiveTrace(TargetObjects, ThisHitPos, RefVec, Light, TargetObjects[TargetNum].get(), ++Count);
					if (Result.first.IsHit)
					{
						HitData.second = HitData.second * Result.second;
					}
				}
				for (auto& CheckShadowObject : TargetObjects)
				{
					if (CheckShadowObject.get() == TargetObjects[TargetNum].get())
					{
						continue;
					}

					Vector3 ShadowTmpNormal(0, 0, 0);
					Vector3 ShadowTmpPos(0, 0, 0);
					if (CheckShadowObject->CheckHit(TmpHitPos, RLight, ShadowTmpNormal, ShadowTmpPos))
					{
						HitData.second.Red *= 0.5f;
						HitData.second.Blue *= 0.5f;
						HitData.second.Green *= 0.5f;
						break;
					}
				}

				HitDatas.emplace_back(HitData);
			}
		}

		

		//Zバッファ
		if (HitDatas.size() > 0)
		{
			Color DrawCol(0, 0, 0);
			float Distance = -1;
			for (auto& HitData : HitDatas)
			{
				if (Distance == -1)
				{
					DrawCol = HitData.second;
					Distance = HitData.first.HitDistance;
					continue;
				}

				if (Distance > HitData.first.HitDistance)
				{
					DrawCol = HitData.second;
					Distance = HitData.first.HitDistance;
				}
			}
			ReturnPair.second = DrawCol;
			ReturnPair.first.IsHit = true;
		}

		return ReturnPair;
}

void RayTracing(const Position3& eye, std::vector<std::shared_ptr<GeometoryObject>>& Objects)
{
	Vector3 Light = BaseLight;
	
	static std::array<std::array<Color, screen_height>, screen_width> PixelCash;
	int ZSpeed = 1;
	int YSpeed = 1;
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		Position3 AddPos(0,YSpeed , ZSpeed);

		Objects[1]->AddPos(AddPos);
		Position3 NowPos = Objects[1]->GetPos();
		YSpeed = (NowPos.y < -200 || NowPos.y>200) ? YSpeed * -1 : YSpeed;
		ZSpeed = (NowPos.z < -300 || NowPos.z>50) ? ZSpeed * -1 : ZSpeed;
		float ScreenWidthHarf = (screen_width / 2);
		float ScreenHeightHarf = (screen_height / 2);
#pragma omp parallel for
		for (int y = 0; y < screen_height; ++y)
		{//スクリーン縦方向
			for (int x = 0; x < screen_width; ++x)
			{//スクリーン横方向
				PixelCash[x][y] = Color(124, 160, 124);
			}
		}

#pragma omp parallel for
		for (int y = 0; y < screen_height; ++y) 
		{//スクリーン縦方向
			for (int x = 0; x < screen_width; ++x) 
			{//スクリーン横方向

				Vector3 Ray = Vector3(x - ScreenWidthHarf, -y + ScreenHeightHarf, 0) - eye;
				Ray.Normalize();

				Position3 HitPos(0, 0, 0);
				Vector3 Normal(0, 0, 0);


				std::vector<std::pair<GeometoryObject::HitInfo, Color>> HitDatas;
				auto RLight = BaseLight * -1;
				RLight.Normalize();
				for (int TargetNum=0;TargetNum<Objects.size();TargetNum++)
				{
					auto HitData = Objects[TargetNum]->IsHitRayAndObject(eye, Ray, Light, Normal, HitPos);
					if (HitData.first.IsHit)
					{
						if (Objects[TargetNum]->GetReflectance() > 0)
						{
							Position3 ThisHitPos = HitPos;
							auto RefVec = GetReflectVector(Ray, Normal).Normalized();
							auto Result = RecursiveTrace(Objects, ThisHitPos, RefVec, Light, Objects[TargetNum].get(), 0);
							if (Result.first.IsHit)
							{
								HitData.second.BlendColor(Result.second, Objects[TargetNum]->GetReflectance());
							}
						}
						for (auto& CheckShadowObject : Objects)
						{
							if (CheckShadowObject.get() == Objects[TargetNum].get())
							{
								continue;
							}

							Vector3 TmpNormal(0, 0, 0);
							Vector3 TmpPos(0, 0, 0);
							if (CheckShadowObject->CheckHit(HitPos, RLight, TmpNormal, TmpPos))
							{
								HitData.second.Red *= 0.5f;
								HitData.second.Blue *= 0.5f;
								HitData.second.Green *= 0.5f;
								break;
							}
						}

						HitDatas.emplace_back(HitData);
					}
				}


				//Zバッファ
				if (HitDatas.size() > 0)
				{
					Color DrawCol(0, 0, 0);
					float Distance = -1;
					for (auto& HitData : HitDatas)
					{
						if (Distance == -1)
						{
							DrawCol = HitData.second;
							Distance = HitData.first.HitDistance;
							continue;
						}

						if (Distance > HitData.first.HitDistance)
						{
							DrawCol = HitData.second;
							Distance = HitData.first.HitDistance;
						}
					}
					PixelCash[x][y] = DrawCol;
				}
			}
		}

	
		for (int y = 0; y < screen_height; ++y)
		{//スクリーン縦方向
			for (int x = 0; x < screen_width; ++x)
			{//スクリーン横方向
				DrawPixelWithColor(x, y, PixelCash[x][y]);
			}
		}
	}
}

void DrawBackGround(float r, float g, float b)
{
	for (int y = 0; y < screen_height; ++y) 
	{//スクリーン縦方向
		for (int x = 0; x < screen_width; ++x) 
		{//スクリーン横方向
			DrawPixel(x, y, GetColor(r, g, b));
		}
	}
}

int main() {
	ChangeWindowMode(true);
	SetGraphMode(screen_width, screen_height, 32);
	SetMainWindowText(_T("Techno1109"));
	DxLib_Init();

	std::vector<std::shared_ptr<GeometoryObject>> Objects;

	Objects.emplace_back(new Plane(0, 1, 0, -100, 50));
	Objects.emplace_back(new Sphere(100, Position3(0, -100, -100), 1.0f, 0.7f, 0.7f, 0.7f));
	Objects.emplace_back(new Sphere(100, Position3(200, 100, -150), 0.7f, 1.0f, 0.7f, 0.5f));
	Objects.emplace_back(new Sphere(100, Position3(-100, 150, -150), 0.7f, 0.7f, 1.0f, 0.6f));
	RayTracing(Vector3(0, 100, 300), Objects);

	DxLib_End();
}