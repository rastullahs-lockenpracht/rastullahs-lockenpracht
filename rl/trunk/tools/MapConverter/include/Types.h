/*
Half-Life MAP to Quake3 Map Conversion Utility
Copyright (C) 2004  Henrik Hinrichs

based on the
Half-Life MAP viewing utility.
Copyright (C) 2003  Ryan Samuel Gregg

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/


#pragma once

typedef __nogc struct Vertex2f
{
	float X;
	float Y;
} Vertex2f;

typedef __nogc struct Vertex2i
{
	int X;
	int Y;
} Vertex2i;

typedef __nogc struct Vertex3f
{
	float X;
	float Y;
	float Z;
} Vertex3f;

typedef __nogc struct Color3f
{
	float R;
	float G;
	float B;
} Color3f;

typedef __nogc struct Color4f
{
	float R;
	float G;
	float B;
	float A;
} Color4f;

typedef __nogc struct Color3uc
{
	unsigned char R;
	unsigned char G;
	unsigned char B;
} Color3uc;

typedef __nogc struct Color4uc
{
	unsigned char R;
	unsigned char G;
	unsigned char B;
	unsigned char A;
} Color4uc;

typedef __gc struct Texture
{
	Texture(String *TextureName)
	{
		this->Loaded = false;
		this->ID = 0;
		this->Width = 1;
		this->Height = 1;
		this->TextureName = TextureName;
	}

	bool Loaded;
	unsigned int ID;
	unsigned long Width;
	unsigned long Height;
	String *TextureName;
} Texture;

typedef __value struct BoundingSphere
{
	Vertex3f vOrigin;
	float fRadius;
} BoundingSphere;

typedef __value struct BoundingBox
{
	Vertex3f vNegBound;
	Vertex3f vPosBound;
} BoundingBox;

__gc class LogManager
{
private:
    static LogManager* m_LogManager = NULL;
    System::IO::StreamWriter* m_streamWriter;
public:
    LogManager() 
    { 
        m_streamWriter = System::IO::File::AppendText("log.txt");
        m_streamWriter->WriteLine(S"");
        m_streamWriter->WriteLine(S"----------------------------------------");
        m_streamWriter->WriteLine(S"");
    };

    void Log(String* str, Color color)
    {
        m_streamWriter->WriteLine(S"{0} {1} : {2}", System::DateTime::Now.ToLongTimeString(),
            System::DateTime::Now.ToLongDateString(),  str );
        m_streamWriter->Flush(); 
    }

    static LogManager* getSingletonPtr()
    {
        if( m_LogManager == NULL )
        {
            m_LogManager = new LogManager();
        }

        return m_LogManager;
    }
};


__gc class Vector
{
public:
	float X;
	float Y;
	float Z;

public:
	// Constructors
	Vector()
	{
		this->X = 0.0f;
		this->Y = 0.0f;
		this->Z = 0.0f;
	}

	Vector(const float X, const float Y, const float Z)
	{
		this->X = X;
		this->Y = Y;
		this->Z = Z;
	}

	Vector(const Vertex3f Vertex)
	{
		this->X = Vertex.X;
		this->Y = Vertex.Y;
		this->Z = Vertex.Z;
	}

	// Operators
	static inline Vector *op_Addition (Vector *Left, Vector *Right)
	{
		return new Vector(Left->X + Right->X, Left->Y + Right->Y, Left->Z + Right->Z);
	}

	static inline Vector *op_Subtraction (Vector *Left, Vector *Right)
	{
		return new Vector(Left->X - Right->X, Left->Y - Right->Y, Left->Z - Right->Z);
	}

	static inline Vector *op_Multiply (Vector *Left, const float Right)
	{
		return new Vector(Left->X * Right, Left->Y * Right, Left->Z * Right);
	}

	static inline bool op_Equality (Vector *Left, Vector *Right)
	{
		return Left->X == Right->X && Left->Y == Right->Y && Left->Z == Right->Z;
	}

	static inline bool op_Inequality (Vector *Left, Vector *Right)
	{
		return Left->X != Right->X || Left->Y != Right->Y || Left->Z != Right->Z;
	}

	// Functions
	void Move(float X, float Y, float Z)
	{
		this->X += X;
		this->Y += Y;
		this->Z += Z;
	}

	inline float Length()
	{
		return (float)Math::Sqrt((double)(X * X + Y * Y + Z * Z));
	}

	inline void Normalize()
	{
		float fLength = this->Length();

		if(fLength != 0.0f)
			fLength = 1.0f / fLength;

		X *= fLength;
		Y *= fLength;
		Z *= fLength;
	}

	inline float Dot(const Vector *V)
	{
		return X * V->X + Y * V->Y + Z * V->Z;
	}

	inline Vector *Cross(const Vector *V)
	{
		return new Vector(Y * V->Z - Z * V->Y, Z * V->X - X * V->Z, X * V->Y - Y * V->X);
	}

	String *ToString()
	{
		return String::Concat(X.ToString(), S", ", Y.ToString(), S", ", Z.ToString());
	}
};

__gc class VectorMath
{
private:
	static Vector *BestAxisLookUp [] = { new Vector(0.0f, 0.0f, 1.0f), new Vector(1.0f, 0.0f, 0.0f), new Vector(0.0f, -1.0f, 0.0f),		// Floor
										 new Vector(0.0f, 0.0f, -1.0f), new Vector(1.0f, 0.0f, 0.0f), new Vector(0.0f, -1.0f, 0.0f),	// Ceiling
										 new Vector(1.0f, 0.0f, 0.0f), new Vector(0.0f, 1.0f, 0.0f), new Vector(0.0f, 0.0f, -1.0f),		// West
										 new Vector(-1.0f, 0.0f, 0.0f), new Vector(0.0f, 1.0f, 0.0f), new Vector(0.0f, 0.0f, -1.0f),	// East
										 new Vector(0.0f, 1.0f, 0.0f), new Vector(1.0f, 0.0f, 0.0f), new Vector(0.0f, 0.0f, -1.0f),		// South
										 new Vector(0.0f, -1.0f, 0.0f), new Vector(1.0f, 0.0f, 0.0f), new Vector(0.0f, 0.0f, -1.0f) };	// North

public:
	static Vector *Cross(Vector *Left, Vector *Right)
	{
		return Left->Cross(Right);
	}

	static float Dot(Vector *Left, Vector *Right)
	{
		return Left->Dot(Right);
	}

	static Vector *MakeOrigin(ArrayList *Vectors)
	{
		Vector *Temp;
		float X = 0.0f, Y = 0.0f, Z = 0.0f, Inv = 0.0f;

		for(int i = 0; i < Vectors->Count; i++)
		{
			Temp = static_cast<Vector*>(Vectors->get_Item(i));
			X += Temp->X;
			Y += Temp->Y;
			Z += Temp->Z;
		}

		Inv = 1.0f / (float)Vectors->Count;

		X *= Inv;
		Y *= Inv;
		Z *= Inv;

		return new Vector(X, Y, Z);
	}

	static bool PointInVectors(ArrayList *Vectors, Vector *Point)
	{
		Vector *Other;

		for(int i = 0; i < Vectors->Count; i++)
		{
			Other = static_cast<Vector*>(Vectors->get_Item(i));

			if(Point->X <= Other->X - EPSILON || Point->X >= Other->X + EPSILON)
				continue;

			if(Point->Y <= Other->Y - EPSILON || Point->Y >= Other->Y + EPSILON)
				continue;

			if(Point->Z <= Other->Z - EPSILON || Point->Z >= Other->Z + EPSILON)
				continue;

			return true;
		}

		return false;
	}

	static bool PointInWorld(Vector *Point)
	{
		if(Point->X < -WORLD || Point->X > WORLD)
			return false;

		if(Point->Y < -WORLD || Point->Y > WORLD)
			return false;

		if(Point->Z < -WORLD || Point->Z > WORLD)
			return false;

		return true;
	}

	static void FlipVectors(ArrayList *Vectors)
	{
		Object *Temp;

		for(int i = 0; i < Vectors->Count / 2; i++)
		{
			Temp = Vectors->get_Item(i);
			Vectors->set_Item(i, Vectors->get_Item(Vectors->Count - 1 - i));
			Vectors->set_Item(Vectors->Count - 1 - i, Temp);
		}
	}

	static Vector *CalculateNormal(Vector *V0, Vector *V1, Vector *V2)
	{
		Vector *Normal = Cross(Vector::op_Subtraction(V2, V1), Vector::op_Subtraction(V0, V1));
		Normal->Normalize();

		return Normal;
	}

	static Vector *CalculateNormal(ArrayList *Vectors)
	{
		if(Vectors->Count < 3)
			return new Vector(0.0f, 0.0f, 0.0f);

		return CalculateNormal(static_cast<Vector*>(Vectors->get_Item(0)), static_cast<Vector*>(Vectors->get_Item(1)), static_cast<Vector*>(Vectors->get_Item(2)));
	}

	static void BestAxis(Vector *Normal, Vector **UAxis, Vector **VAxis)
	{
			int iBestAxis = 0;
			float fDot, fBest = 0.0f;

			for(int i = 0; i < 6; i++)
			{
				fDot = Normal->Dot(BestAxisLookUp[i * 3]);
				if(fDot > fBest)
				{
					fBest = fDot;
					iBestAxis = i * 3;
				}
			}

			*UAxis = BestAxisLookUp[++iBestAxis];
			*VAxis = BestAxisLookUp[++iBestAxis];
	}

	static void ArrangeCCW(Vector *Origin, ArrayList *Vectors)
	{
		if(Vectors->Count < 3)
			return;

		Vector *VA = static_cast<Vector*>(Vectors->get_Item(0)), *VB = static_cast<Vector*>(Vectors->get_Item(1)), *VZ = static_cast<Vector*>(Vectors->get_Item(Vectors->Count - 1));
		Vector *M, *MA, *MB, *MZ, *CMAMB, *CMAMZ;

		M = MakeOrigin(Vectors);

		// Get vectors from the middle point to the points surrounding our starting vertex (A).
		MA = Vector::op_Subtraction(M, VA);
		MB = Vector::op_Subtraction(M, VB);
		MZ = Vector::op_Subtraction(M, VZ);

		// Find out which direction they point.
		CMAMB = MA->Cross(MB);
		CMAMZ = MA->Cross(MZ);

		// Normalize this and add it to the middle of the face.
		CMAMB->Normalize();
		CMAMZ->Normalize();

		CMAMB =  Vector::op_Addition(CMAMB, M);
		CMAMZ =  Vector::op_Addition(CMAMZ, M);

		// See which one is farther from the origin.
		MB = Vector::op_Subtraction(Origin, CMAMB);
		MZ = Vector::op_Subtraction(Origin, CMAMZ);

		// If B is farther the elements are CW, swap the elements.
		if(MB->Length() > MZ->Length())
		{
			FlipVectors(Vectors);
		}
	}

	static void SortVectors(ArrayList *Vectors, Vector *N)
	{
		Vector *M, *MA, *MB;
		Object *Temp;

		int iSmallestAngle;
		float fAngle, fSmallestAngle;
		M = MakeOrigin(Vectors);

		for(int i = 0; i < Vectors->Count - 2; i++)
		{
			iSmallestAngle = -1;
			fSmallestAngle = -1.0f;

			MA = Vector::op_Subtraction(static_cast<Vector*>(Vectors->get_Item(i)), M);
			MA->Normalize();
			
			for(int j = i + 1; j < Vectors->Count; j++)
			{
				MB = Vector::op_Subtraction(static_cast<Vector*>(Vectors->get_Item(j)), M);
				MB->Normalize();

				//  3 = 360 deg
				//  2 = 270 deg
				//  1 = 180 deg
				//  0 = 90 deg
				// -1 = 0 deg
				fAngle = MA->Dot(MB);

				if(Cross(MA, MB)->Dot(N) < 0.0f)
				{
					fAngle += 2.0f;
				}
				
				if(fAngle > fSmallestAngle)
				{
					fSmallestAngle = fAngle;
					iSmallestAngle = j;
				}
			}

			if(iSmallestAngle != i + 1 && iSmallestAngle != -1)
			{
				Temp = Vectors->get_Item(i + 1);
				Vectors->set_Item(i + 1, Vectors->get_Item(iSmallestAngle));
				Vectors->set_Item(iSmallestAngle, Temp);
			}
		}
	}
};