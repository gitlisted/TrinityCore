#include "Geometry.h"
#include "Constants.h"

Geometry::Geometry() : Transform(false)
{
    Vertices.reserve(10000);
    Triangles.reserve(10000);
}

void Geometry::CalculateBoundingBox( float*& min, float*& max )
{
    min = new float[3];
    max = new float[3];

    for (std::vector<Vector3>::iterator itr = Vertices.begin(); itr != Vertices.end(); ++itr)
    {
        if (itr->x > max[0])
            max[0] = itr->x;
        if (itr->x < min[0])
            min[0] = itr->x;

        if (itr->y > max[1])
            max[1] = itr->y;
        if (itr->y < min[1])
            min[1] = itr->y;

        if (itr->z > max[2])
            max[2] = itr->z;
        if (itr->z < min[2])
            min[2] = itr->z;
    }
}

void Geometry::CalculateMinMaxHeight( float& min, float& max )
{
    min = 0.0f;
    max = 0.0f;

    for (std::vector<Vector3>::iterator itr = Vertices.begin(); itr != Vertices.end(); ++itr)
    {
        if (Transform)
        {
            if (itr->y < min)
                min = itr->y;
            if (itr->y > max)
                max = itr->y;
        }
        else
        {
            if (itr->z < min)
                min = itr->z;
            if (itr->z > max)
                max = itr->z;
        }
    }
}

void Geometry::AddData( std::vector<Vector3>& verts, std::vector<Triangle<uint32> >& tris )
{
    uint32 vertOffset = Vertices.size();
    for (std::vector<Vector3>::iterator itr = verts.begin(); itr != verts.end(); ++itr)
        Vertices.push_back(Transform ? Utils::ToRecast(*itr) : *itr);
    
    for (std::vector<Triangle<uint32> >::iterator itr = tris.begin(); itr != tris.end(); ++itr)
        Triangles.push_back(Triangle<uint32>(itr->Type, itr->V0 + vertOffset, itr->V1 + vertOffset, itr->V2 + vertOffset));
}

void Geometry::GetRawData( float*& verts, int*& tris, uint8*& areas )
{
    verts = new float[Vertices.size() * 3];
    for (int i = 0; i < Vertices.size(); ++i)
    {
        Vector3& vert = Vertices[i];
        verts[(i * 3) + 0] = vert.x;
        verts[(i * 3) + 1] = vert.y;
        verts[(i * 3) + 2] = vert.z;
    }

    tris = new int[Triangles.size() * 3];
    for (int i = 0; i < Triangles.size(); ++i)
    {
        Triangle<uint32>& tri = Triangles[i];
        tris[(i * 3) + 0] = (int)tri.V0;
        tris[(i * 3) + 1] = (int)tri.V1;
        tris[(i * 3) + 2] = (int)tri.V2;
    }

    areas = new uint8[Triangles.size()];
    for (int i = 0; i < Triangles.size(); i++)
    {
        switch (Triangles[i].Type)
        {
            case Constants::TRIANGLE_TYPE_WATER:
                areas[i] = Constants::POLY_AREA_WATER;
                break;
            default:
                areas[i] = Constants::POLY_AREA_TERRAIN;
                break;
        }
    }
}
