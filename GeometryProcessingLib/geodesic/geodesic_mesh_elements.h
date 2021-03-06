/************************************************************************/
/* Amended Danil Kirsanov's geodesic mesh structure to CGAL HDS         */
/* elements used to calculate geodesic are defined						*/
/************************************************************************/
#pragma once

#include <assert.h>
#include <cstddef>
#include "MeshElementBase.h"
#include "Another_HDS_model.h"
#include "Another_CGAL_cast.h"

namespace Another
{
	/************************************************************************/
	/* /brief point on the surface of the mesh                              */
	/************************************************************************/
	class Surface_point  
	{
	public:
		Surface_point():
		  m_p(NULL)
		  {}

		  Surface_point(Vertex_handle v)
		  {//set the surface point in the vertex
			  m_position = v->point();
			  m_p = Another_CGAL_cast::vertex_cast(v);
		  }

		  //set the surface point in the center of the face
		  Surface_point(Face_handle f)
		  {
			  m_p = Another_CGAL_cast::face_cast(f);
			  vector<Vertex_handle> vs;
			  f->get_all_vertex(vs);
			  float center_data[3];
			  center_data[0] = 0.0f;
			  center_data[1] = 0.0f;
			  center_data[2] = 0.0f;

			  for (unsigned int i = 0; i< vs.size(); i++)
			  {
				  Vertex_handle tmpv = vs[i];
				  Point_3 tmpp = tmpv->point();
				  center_data[0] = center_data[0] + tmpp[0];
				  center_data[1] = center_data[1] + tmpp[1];
				  center_data[0] = center_data[2] + tmpp[2];
			  }
			  for (size_t i = 0; i < 3; i++)
			  {
				  center_data[i] = center_data[i]/3.0;
			  }
			  m_position = 	Point_3(center_data[0], center_data[1], center_data[2]);
		  }

		  //set the surface point in the middle of the edge
		  Surface_point(Halfedge_handle e, double a = 0.5)
		  {
			  m_p = Another_CGAL_cast::halfedge_cast(e);
			  double b = 1 - a;
			  Vertex_handle v0 = e->vertex();
			  Vertex_handle v1 = e->opposite_vertex();
			  Point_3 p0 = v0->point();
			  Point_3 p1 = v1->point();

			  float center_data[3];
			  center_data[0] = 0.0f;
			  center_data[1] = 0.0f;
			  center_data[2] = 0.0f;
			  for (size_t i = 0; i < 3; i++)
			  {
				  center_data[i] = ( b*p0[i] +  a*p1[i] );
			  }
			  m_position = Point_3(center_data[0], center_data[1], center_data[2]);
		  }

		  Surface_point(Base_handle g, double x, double y, double z,  PointType t = UNDEFINED_POINT_TYPE)
		  {
			  m_p = g;
			  m_position = Point_3(x,y,z);
		  }

		  void initialize(Surface_point const& p)
		  {
			  *this = p;
		  }

		  ~Surface_point(){}

		  PointType type(){return m_p ? m_p->type() : UNDEFINED_POINT_TYPE;}
		  Base_handle& base_element(){return m_p;}
		  double distance(double* v)
		  {
			  double dx = m_position[0] - v[0];
			  double dy = m_position[1] - v[1];
			  double dz = m_position[2] - v[2];

			  return sqrt(dx*dx + dy*dy + dz*dz);
		  }

		  double distance(Point_3 v)
		  {
			  double newPoint[3];
			  newPoint[0] = v[0];
			  newPoint[1] = v[1];
			  newPoint[2] = v[2];
			  return distance(newPoint);
		  }

		  double distance(Surface_point* v)
		  {
			  return distance(v->get_point());
		  }

		  Point_3 get_point()
		  {
			  return m_position;
		  }

	protected:
		Base_handle m_p;			//could be face, vertex or edge pointer
		Point_3 m_position;
	};
}
