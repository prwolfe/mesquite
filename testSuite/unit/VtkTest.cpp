/* ***************************************************************** 
    MESQUITE -- The Mesh Quality Improvement Toolkit

    Copyright 2004 Sandia Corporation and Argonne National
    Laboratory.  Under the terms of Contract DE-AC04-94AL85000 
    with Sandia Corporation, the U.S. Government retains certain 
    rights in this software.

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License 
    (lgpl.txt) along with this library; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 
    diachin2@llnl.gov, djmelan@sandia.gov, mbrewer@sandia.gov, 
    pknupp@sandia.gov, tleurent@mcs.anl.gov, tmunson@mcs.anl.gov      
   
  ***************************************************************** */
// -*- Mode : c++; tab-width: 2; c-tab-always-indent: t; indent-tabs-mode: nil; c-basic-offset: 2 -*-
//

#ifdef MSQ_USE_OLD_IO_HEADERS
#include <iostream.h>
#else
#include <iostream>
using std::cout;
#endif

#ifndef DEBUG
#  include "Mesquite.hpp"
#  include "MeshSet.hpp"
#  include "PatchData.hpp"
#  include "MeshImpl.hpp"
#  include "cppunit/extensions/HelperMacros.h"
   using Mesquite::Mesh;
   using Mesquite::MeshImpl;
   using Mesquite::Vector3D;
   using Mesquite::MsqVertex;
   using Mesquite::MsqPrintError;
   using Mesquite::EntityTopology;
#else
#  include <stdio.h>
#endif

extern const char temp_file_name[] = "VtkTest.vtk";

  // VTK file for 2x2x2 block of hexes as structured-point 
extern const char structured_3d_points_data[] = 
"# vtk DataFile Version 2.0\n"
"Mesquite Mesh\n"
"ASCII\n"
"DATASET STRUCTURED_POINTS\n"
"DIMENSIONS 3 3 3\n"
"ORIGIN 0 0 0\n"
"SPACING 1.5 1.5 1.5\n";

  // VTK file for 2x2 block of quads as structured-point 
extern const char structured_2d_points_data[] = 
"# vtk DataFile Version 2.0\n"
"Mesquite Mesh\n"
"ASCII\n"
"DATASET STRUCTURED_POINTS\n"
"DIMENSIONS 3 3 1\n"
"ORIGIN 0 0 0\n"
"SPACING 1.5 1.5 0.0\n";

  // VTK file for 2x2x2 block of hexes as structured-grid
extern const char structured_grid_data[] = 
"# vtk DataFile Version 2.0\n"
"Mesquite Mesh\n"
"ASCII\n"
"DATASET STRUCTURED_GRID\n"
"DIMENSIONS 3 3 3\n"
"POINTS 27 float\n"
"0.0 0.0 0.0\n1.5 0.0 0.0\n3.0 0.0 0.0\n"
"0.0 1.5 0.0\n1.5 1.5 0.0\n3.0 1.5 0.0\n"
"0.0 3.0 0.0\n1.5 3.0 0.0\n3.0 3.0 0.0\n"
"0.0 0.0 1.5\n1.5 0.0 1.5\n3.0 0.0 1.5\n"
"0.0 1.5 1.5\n1.5 1.5 1.5\n3.0 1.5 1.5\n"
"0.0 3.0 1.5\n1.5 3.0 1.5\n3.0 3.0 1.5\n"
"0.0 0.0 3.0\n1.5 0.0 3.0\n3.0 0.0 3.0\n"
"0.0 1.5 3.0\n1.5 1.5 3.0\n3.0 1.5 3.0\n"
"0.0 3.0 3.0\n1.5 3.0 3.0\n3.0 3.0 3.0\n";

  // VTK file for 2x2x2 block of hexes as rectilinear-grid
extern const char rectilinear_grid_data[] =
"# vtk DataFile Version 2.0\n"
"Mesquite Mesh\n"
"ASCII\n"
"DATASET RECTILINEAR_GRID\n"
"DIMENSIONS 3 3 3\n"
"X_COORDINATES 3 float\n"
"0.0 1.5 3.0\n"
"Y_COORDINATES 3 float\n"
"0.0 1.5 3.0\n"
"Z_COORDINATES 3 float\n"
"0.0 1.5 3.0\n";

  // VTK file containing mixed-element unstructured mesh
  // First 8 elems result in same 2x2x2 block of hexes as
  // structured cases above.  The next 4 elems are the same
  // as the quads from the 2D structured-point file above.
extern const char mixed_unstructured_data[] = 
"# vtk DataFile Version 2.0\n"
"Mesquite Mesh\n"
"ASCII\n"
"DATASET UNSTRUCTURED_GRID\n"
"POINTS 35 float\n"
"\n" // points for an 2x2x2 brick of hexes (same geom/topo as above structured meshes)
"0.0 0.0 0.0\n1.5 0.0 0.0\n3.0 0.0 0.0\n"
"0.0 1.5 0.0\n1.5 1.5 0.0\n3.0 1.5 0.0\n"
"0.0 3.0 0.0\n1.5 3.0 0.0\n3.0 3.0 0.0\n"
"0.0 0.0 1.5\n1.5 0.0 1.5\n3.0 0.0 1.5\n"
"0.0 1.5 1.5\n1.5 1.5 1.5\n3.0 1.5 1.5\n"
"0.0 3.0 1.5\n1.5 3.0 1.5\n3.0 3.0 1.5\n"
"0.0 0.0 3.0\n1.5 0.0 3.0\n3.0 0.0 3.0\n"
"0.0 1.5 3.0\n1.5 1.5 3.0\n3.0 1.5 3.0\n"
"0.0 3.0 3.0\n1.5 3.0 3.0\n3.0 3.0 3.0\n"
"\n" // more points on +x side of brick for pyramids and tets
"4 0.75 0.75\n4 2.25 0.75\n4 0.75 2.25\n4 2.25 2.25\n"
"\n" // more points for two prisms/wedges
"6 0.75 0.75\n6 2.25 0.75\n6 0.75 2.25\n6 2.25 2.25\n"
"\n"
"CELLS 38 216\n"
"\n" // 8 hexes in 2x2x2 block
"8  0  1  4  3  9 10 13 12\n"
"8  1  2  5  4 10 11 14 13\n"
"8  3  4  7  6 12 13 16 15\n"
"8  4  5  8  7 13 14 17 16\n"
"8  9 10 13 12 18 19 22 21\n"
"8 10 11 14 13 19 20 23 22\n"
"8 12 13 16 15 21 22 25 24\n"
"8 13 14 17 16 22 23 26 25\n"
"\n"// Quads on -z face of hex (inverted to match structured data)
"4 0 1 4 3\n" 
"4 1 2 5 4\n"
"4 3 4 7 6\n"
"4 4 5 8 7\n"
"\n" // some pyramids on the +x side of the block
"5  2  5 14 11 27\n" 
"5  5  8 17 14 28\n"
"5 11 14 23 20 29\n"
"5 14 17 26 23 30\n"
"\n" // Some tetrahedrons around the pyramids
"4  5 14 27 28\n"
"4 14 28 17 30\n"
"4 29 14 23 30\n"
"4 11 27 14 29\n"
"4 27 29 30 14\n"
"4 28 27 30 14\n"
"\n" // Triangles bounding the pyramid/tet region
"3  2  5 27\n"
"3  5 28 27\n"
"3  5  8 28\n"
"3  8 17 28\n"
"3 17 30 28\n"
"3 17 26 30\n"
"3 26 23 30\n"
"3 23 29 30\n"
"3 23 20 29\n"
"3 20 11 29\n"
"3 11 27 29\n"
"3  2 27 11\n"
"3 27 28 30\n"
"3 27 30 29\n"
"\n" // A couple wedges/prisms
"6 27 30 28 31 34 32\n"
"6 30 27 29 34 31 33\n"
"\n" 
"CELL_TYPES 38\n"
"12 12 12 12 12 12 12 12\n"  //  8 hexes
" 9  9  9  9\n"              //  4 quads
"14 14 14 14\n"              //  4 pyramids
"10 10 10 10 10 10\n"        //  6 tets
" 5  5  5  5  5  5  5 "
" 5  5  5  5  5  5  5\n"     // 14 tri
"13 13\n";                   // 2 wedges

extern const char quadratic_unstructured_data[] = 
"# vtk DataFile Version 2.0\n"
"Mesquite Mesh\n"
"ASCII\n"
"DATASET UNSTRUCTURED_GRID\n"
"POINTS 30 float\n"
"\n" // points for a single quadtratic hex, in ExodusII order
" 1.0 -1.0 -1.0 \n  1.0  1.0 -1.0 \n -1.0  1.0 -1.0 \n -1.0 -1.0 -1.0 \n"  // bottom corners
" 1.0 -1.0  1.0 \n  1.0  1.0  1.0 \n -1.0  1.0  1.0 \n -1.0 -1.0  1.0 \n"  // top corners
" 1.0  0.0 -1.0 \n  0.0  1.0 -1.0 \n -1.0  0.0 -1.0 \n  0.0 -1.0 -1.0 \n"  // bottom mid-nodes
" 1.0 -1.0  0.0 \n  1.0  1.0  0.0 \n -1.0  1.0  0.0 \n -1.0 -1.0  0.0 \n"  // side mid-nodes 
" 1.0  0.0  1.0 \n  0.0  1.0  1.0 \n -1.0  0.0  1.0 \n  0.0 -1.0  1.0 \n"  // top mid-nodes
"\n" // points for a single quadtatic tet, in ExodusII order
" 1.0 -1.0 -1.0 \n  1.0  1.0 -1.0 \n -1.0  0.0 -1.0 \n" // base triangle
" 0.0  0.0  1.0 \n "                                    // apex
" 1.0  0.0 -1.0 \n  0.0  0.5 -1.0 \n  0.0 -0.5 -1.0 \n" // base mid-nodes
" 0.5 -0.5  0.0 \n  0.5  0.5  0.0 \n -0.5  0.0  0.0 \n" // side mid-nodes
"\n"
"CELLS 4 48\n"
"20 0 1 2 3 4 5 6 7 8 9 10 11 16 17 18 19 12 13 14 15\n" // Hex20
"10 20 21 22 23 24 25 26 27 28 29\n" // Tet10
"8 0 1 2 3 8 9 10 11\n" // Quad8 (base of hex)
"6 20 21 22 24 25 26\n" // Tri6 (base of tet)
"\n"
"CELL_TYPES 4\n"
"25 24 23 22\n";


  // A simple scalar attribute specifying point and hex
  // IDs.  May be appended to any of the above 3D structured
  // mesh files.
extern const char simple_scalar_attrib[] = 
"CELL_DATA 8\n"
"SCALARS global_id int 1\n"
"LOOKUP_TABLE default\n"
"1 2 3 4 5 6 7 8\n"
"\n"
"POINT_DATA 27\n"
"SCALARS global_id int\n"
"LOOKUP_TABLE default\n"
" 1  2  3  4  5  6  7  8  9\n"
"10 11 12 13 14 15 16 17 18\n"
"19 20 21 22 23 24 25 26 27\n";

  // A VTK vector attribute.  May be appended to any of the 
  // above 3D structured mesh files.
extern const char simple_vector_attrib[] =
"CELL_DATA 8\n"
"VECTORS hexvect float\n"
"1 1 1\n"
"2 2 2\n"
"3 3 3\n"
"4 4 4\n"
"5 5 5\n"
"6 6 6\n"
"7 7 7\n"
"8 8 8\n";

  // A scalar VTK attribute with the name and datatype
  // expected by MeshImpl for specifying boundary vertices.
  // May be appended to any of the above 3D structured
  // mesh files
extern const char fixed_vertex_attrib[] = 
"POINT_DATA 27\n"
"SCALARS fixed float\n"
"LOOKUP_TABLE default\n"
"1 1 1 1 1 1 1 1 1\n"
"1 1 1 1 0 1 1 1 1\n"
"1 1 1 1 1 1 1 1 1\n";

#ifndef DEBUG

class VtkTest : public CppUnit::TestFixture
{
private:
   CPPUNIT_TEST_SUITE(VtkTest);

    // Original test for old Vtk parser
   CPPUNIT_TEST (test_elements);

    // Additional tests for new Vtk parser - J.Kraftcheck, 2004-10-12
   CPPUNIT_TEST (test_read_unstructured);
   CPPUNIT_TEST (test_read_structured_2d_points);
   CPPUNIT_TEST (test_read_structured_3d_points);
   CPPUNIT_TEST (test_read_structured_grid);
   CPPUNIT_TEST (test_read_rectilinear_grid);
   CPPUNIT_TEST (test_read_simple_scalar_attrib);
   CPPUNIT_TEST (test_read_vector_attrib);
   CPPUNIT_TEST (test_read_fixed_attrib);
   CPPUNIT_TEST (test_read_quadratic);
   CPPUNIT_TEST (test_write_quadratic);
   
    // Test writer - J.Kraftcheck, 2004-10-12
   CPPUNIT_TEST (test_write);

   CPPUNIT_TEST_SUITE_END();
   
public:
   /* Automatically called by CppUnit before each test function. */
  void setUp()
  {
  }
  
    // Automatically called by CppUnit after each test function.
  void tearDown()
  {
  }
  
public:
  VtkTest()
    {}
  
    // Check if the 2x2x2 brick of structured mesh
    // read from file is as expected.
  void check_8hex_structured( Mesh& mesh );
  
    // Check if the 2x2x2 brick of hexes
    // read from file is as expected.
  void check_8hex_block( Mesh& mesh, std::vector<Mesh::VertexHandle>::iterator connectivity );
  
    // Check if the 2x2 brick of structured mesh
    // read from file is as expected.
  void check_4quad_structured( Mesh& mesh );
  
    // Check if the 2x2 brick of quads
    // read from file is as expected.
  void check_4quad_block( Mesh& mesh, std::vector<Mesh::VertexHandle>::iterator connectivity );
        
    
    // Test reading VTK unstructured mesh
  void test_read_unstructured();
  
  void test_read_unstructured( const char* filename );
  
  
    // Test reading 2D Vtk structured-points mesh
  void test_read_structured_2d_points();
  
  
    // Test reading 3D Vtk structured-points mesh
  void test_read_structured_3d_points();
  
  
    // Test reading 3D Vtk structured-grid mesh
  void test_read_structured_grid();
  
  
    // Test reading 3D Vtk rectilinear-grid mesh
  void test_read_rectilinear_grid();
  
  
    // Test reading Vtk simple (one-component) scalar attribute
  void test_read_simple_scalar_attrib();
   
  
    // Test reading Vtk vector attribute
  void test_read_vector_attrib();
  

    // Test reading MeshImpl boundary-vertex bit
    // from Vtk scalar attribute.
  void test_read_fixed_attrib();
 
    
    // Test writing VTK unstructured mesh
  void test_write();
  
  void test_read_quadratic();
  void test_read_quadratic(const char* filename);
  void test_write_quadratic();
  
  void test_elements();
  
  int tri_check_validity(Mesquite::MsqMeshEntity* element_array,
                         size_t num_elements,
                         Mesquite::MsqVertex* vtx_array,
                         size_t num_vertices);
  
  int tet_validity_check(Mesquite::MsqMeshEntity* element_array,
                         size_t num_elements,
                         Mesquite::MsqVertex *vtx_array);
};
    // Check if the 2x2x2 brick of structured mesh
    // read from file is as expected.
  void VtkTest::check_8hex_structured( Mesh& mesh )
  {
    MsqPrintError err(cout);
    size_t vertex_count, element_count, use_count;
    mesh.get_all_sizes( vertex_count, element_count, use_count, err );
    CPPUNIT_ASSERT(!err && vertex_count == 27 && element_count == 8 && use_count == 64);
    Mesh::ElementHandle elems[8];
    Mesh::VertexHandle verts[27];
    size_t uses[64], offsets[9];
    mesh.get_all_mesh( verts, 27, elems, 8, offsets, 9, uses, 64, err );
    CPPUNIT_ASSERT(!err);
    
    
    std::vector<Mesh::VertexHandle> connectivity(8*8);
    std::vector<Mesh::VertexHandle>::iterator iter = connectivity.begin();
    for (int i = 0; i < 8; ++i)
    {
      Mesh::ElementHandle elem = elems[i];
      size_t junk1[8], junk2[8];
      size_t junk1size = sizeof(junk1) / sizeof(size_t);
      size_t junk2size = sizeof(junk2) / sizeof(size_t);
      mesh.elements_get_attached_vertices( &elem, 1, &*iter, junk1size, junk1, junk2size, junk2, err );
      CPPUNIT_ASSERT(!err);
      iter += 8;
    }
    
    check_8hex_block( mesh, connectivity.begin() );
  }
  
    // Check if the 2x2x2 brick of hexes
    // read from file is as expected.
  void VtkTest::check_8hex_block( Mesh& mesh, std::vector<Mesh::VertexHandle>::iterator connectivity )
  {
    MsqPrintError err(cout);
    const int base_corners[] =   { 0, 1, 3, 4, 9, 10, 12, 13 };
    const int corner_offsets[] = { 0, 1, 4, 3, 9, 10, 13, 12 };

    for (int hex = 0; hex < 8; ++hex)
    {
      for (int node = 0; node < 8; ++node)
      {
        const int index = base_corners[hex] + corner_offsets[node];
        const int x = index % 3;
        const int y = (index / 3) % 3;
        const int z = index / 9;
        const Vector3D expected_coords( 1.5*x, 1.5*y, 1.5*z );
        MsqVertex actual_coords;
        Mesh::VertexHandle* conn_ptr = &*connectivity;
        ++connectivity;
        mesh.vertices_get_coordinates( conn_ptr, &actual_coords, 1, err );
        CPPUNIT_ASSERT( !err );
        CPPUNIT_ASSERT( expected_coords.within_tolerance_box( actual_coords, DBL_EPSILON ) );
      }
    }
  }
  
    // Check if the 2x2 brick of structured mesh
    // read from file is as expected.
  void VtkTest::check_4quad_structured( Mesh& mesh )
  {
    MsqPrintError err(cout);
    size_t vertex_count, element_count, use_count;
    mesh.get_all_sizes( vertex_count, element_count, use_count, err );
    CPPUNIT_ASSERT(!err && vertex_count == 9 && element_count == 4 && use_count == 16);
    Mesh::ElementHandle elems[4];
    Mesh::VertexHandle verts[9];
    size_t uses[16], offsets[5];
    mesh.get_all_mesh( verts, 9, elems, 4, offsets, 5, uses, 16, err );
    CPPUNIT_ASSERT(!err);
    
    
    std::vector<Mesh::VertexHandle> connectivity(4*4);
    std::vector<Mesh::VertexHandle>::iterator iter = connectivity.begin();
    for (int i = 0; i < 4; ++i)
    {
      Mesh::ElementHandle elem = elems[i];
      size_t junk1[4], junk2 = 4, junk3[4];
      mesh.elements_get_attached_vertices( &elem, 1, &*iter, junk2, junk1, junk2, junk3, err );
      CPPUNIT_ASSERT(!err);
      iter += 4;
    }
    
    check_4quad_block( mesh, connectivity.begin() );
  }
  
    // Check if the 2x2 brick of quads
    // read from file is as expected.
  void VtkTest::check_4quad_block( Mesh& mesh, std::vector<Mesh::VertexHandle>::iterator connectivity )
  {
    MsqPrintError err(cout);
    const int base_corners[]   = { 0, 1, 3, 4 };
    const int corner_offsets[] = { 0, 1, 4, 3 };
    for (int quad = 0; quad < 4; ++quad)
    {
      for (int node = 0; node < 4; ++node)
      {
        const int index = base_corners[quad] + corner_offsets[node];
        const int x = index % 3;
        const int y = index / 3;
        const Vector3D expected_coords( 1.5*x, 1.5*y, 0.0 );
        MsqVertex actual_coords;
        Mesh::VertexHandle* conn_ptr = &*connectivity;
        ++connectivity;
        mesh.vertices_get_coordinates( conn_ptr, &actual_coords, 1, err );
        CPPUNIT_ASSERT( !err );
        CPPUNIT_ASSERT( expected_coords.within_tolerance_box( actual_coords, DBL_EPSILON ) );
      }
    }
  }
        
    
    // Test reading VTK unstructured mesh
  void VtkTest::test_read_unstructured()
  {
    FILE* file = fopen( temp_file_name, "w+" );
    CPPUNIT_ASSERT( file );
    int rval = fputs( mixed_unstructured_data, file );
    fclose( file );
    if (rval == EOF) remove( temp_file_name );
    CPPUNIT_ASSERT( rval != EOF );

    test_read_unstructured( temp_file_name );
  }
  
  void VtkTest::test_read_unstructured( const char* filename )
  {
    MeshImpl mesh;
    MsqPrintError err(cout);
    
    mesh.read_vtk( filename, err );
    remove( temp_file_name );
    CPPUNIT_ASSERT(!err);
    
    size_t vertex_count, element_count, use_count;
    mesh.get_all_sizes( vertex_count, element_count, use_count, err );
    CPPUNIT_ASSERT(!err && vertex_count == 35 && element_count == 38);
    std::vector<Mesh::VertexHandle> verts(35);
    std::vector<Mesh::ElementHandle> elems(38);
    std::vector<size_t> uses(use_count), offsets(39);
    mesh.get_all_mesh( &verts[0], verts.size(),
                       &elems[0], elems.size(),
                       &offsets[0], offsets.size(),
                       &uses[0], uses.size(), err );
    CPPUNIT_ASSERT(!err);

    unsigned i;
    struct meshdata { EntityTopology type; size_t nodes; size_t count; };
    meshdata list[] = {
      { Mesquite::HEXAHEDRON,    8,  8 },
      { Mesquite::QUADRILATERAL, 4,  4 },
      { Mesquite::PYRAMID,       5,  4 },
      { Mesquite::TETRAHEDRON,   4,  6 },
      { Mesquite::TRIANGLE,      3, 14 },
      { Mesquite::PRISM,         6,  2 }, 
      { Mesquite::MIXED,         0,  0 } };
      
    size_t conn_len = 0;
    for (i = 0; list[i].nodes; ++i)
      conn_len += list[i].nodes * list[i].count;
    std::vector<Mesh::VertexHandle> connectivity(conn_len);

    std::vector<Mesh::VertexHandle>::iterator c_iter = connectivity.begin();
    std::vector<Mesh::ElementHandle>::iterator e_iter = elems.begin();
    size_t junk1[8], junk2[8];
    for (i = 0; list[i].nodes; ++i)
    {
      for (unsigned j = 0 ; j < list[i].count; ++j)
      {
        CPPUNIT_ASSERT( mesh.element_get_topology( *e_iter, err ) == list[i].type );
        CPPUNIT_ASSERT( !err );
        CPPUNIT_ASSERT( mesh.element_get_attached_vertex_count( *e_iter, err ) == list[i].nodes );
        CPPUNIT_ASSERT( !err );
        size_t junk2size = sizeof(junk2) / sizeof(size_t);
        mesh.elements_get_attached_vertices( &*e_iter, 1, &*c_iter, list[i].nodes, junk1, junk2size, junk2, err );
        CPPUNIT_ASSERT( !err );
        c_iter += list[i].nodes;
        ++e_iter;
      }
    }

    check_8hex_block( mesh, connectivity.begin() );
    c_iter = connectivity.begin();
    c_iter += 64;
    check_4quad_block( mesh, c_iter );
  }
  
  
    // Test reading 2D Vtk structured-points mesh
  void VtkTest::test_read_structured_2d_points()
  {
    MeshImpl mesh;
    MsqPrintError err(cout);
   
    FILE* file = fopen( temp_file_name, "w+" );
    fputs( structured_2d_points_data, file );
    fclose( file );
    
    mesh.read_vtk( temp_file_name, err );
    remove( temp_file_name );
    CPPUNIT_ASSERT(!err);
    
    check_4quad_structured( mesh );
  }
  
  
    // Test reading 3D Vtk structured-points mesh
  void VtkTest::test_read_structured_3d_points()
  {
    MeshImpl mesh;
    MsqPrintError err(cout);
    
    FILE* file = fopen( temp_file_name, "w+" );
    fputs( structured_3d_points_data, file );
    fclose( file );
    
    mesh.read_vtk( temp_file_name, err );
    remove( temp_file_name );
    CPPUNIT_ASSERT(!err);
    
    check_8hex_structured( mesh );
  }
  
  
    // Test reading 3D Vtk structured-grid mesh
  void VtkTest::test_read_structured_grid()
  {
    MeshImpl mesh;
    MsqPrintError err(cout);
    
    FILE* file = fopen( temp_file_name, "w+" );
    fputs( structured_grid_data, file );
    fclose( file );
    
    mesh.read_vtk( temp_file_name, err );
    remove( temp_file_name );
    CPPUNIT_ASSERT(!err);
    
    check_8hex_structured( mesh );
  }
  
  
    // Test reading 3D Vtk rectilinear-grid mesh
  void VtkTest::test_read_rectilinear_grid()
  {
    MeshImpl mesh;
    MsqPrintError err(cout);
    
    FILE* file = fopen( temp_file_name, "w+" );
    fputs( rectilinear_grid_data, file );
    fclose( file );
    
    mesh.read_vtk( temp_file_name, err );
    remove( temp_file_name );
    CPPUNIT_ASSERT(!err);
    
    check_8hex_structured( mesh );
  }
  
  
    // Test reading Vtk simple (one-component) scalar attribute
  void VtkTest::test_read_simple_scalar_attrib()
  {
    MeshImpl mesh;
    MsqPrintError err(cout);
   
    FILE* file = fopen( temp_file_name, "w+" );
    fputs( structured_3d_points_data, file );
    fputs( simple_scalar_attrib, file );
    fclose( file );
    
    mesh.read_vtk( temp_file_name, err );
    remove( temp_file_name );
    CPPUNIT_ASSERT(!err);
    
    size_t num_vert, num_elem, num_uses;
    mesh.get_all_sizes( num_vert, num_elem, num_uses, err );
    CPPUNIT_ASSERT( !err );
    msq_std::vector<Mesh::VertexHandle> verts(num_vert);
    msq_std::vector<Mesh::ElementHandle> elems(num_elem);
    msq_std::vector<size_t> offsets(num_elem+1);
    msq_std::vector<size_t> uses(num_uses);
    mesh.get_all_mesh( &verts[0], verts.size(),
                       &elems[0], elems.size(),
                       &offsets[0], offsets.size(),
                       &uses[0], uses.size(), err );
    CPPUNIT_ASSERT( !err );
   
    void* th = mesh.tag_get( "global_id", err );
    CPPUNIT_ASSERT( !err );

    msq_std::string name;
    Mesh::TagType type;
    unsigned tagsize;
    mesh.tag_properties( th, name, type, tagsize, err );
    CPPUNIT_ASSERT( !err && type == Mesh::INT && tagsize == 1 );
    
    int elem_data[8];
    mesh.tag_get_element_data( th, 8, &elems[0], elem_data, err );
    CPPUNIT_ASSERT( !err );
    
    for (int i = 0; i < 8; ++i)
      CPPUNIT_ASSERT( elem_data[i] == (1+i) );
  }
   
  
    // Test reading Vtk vector attribute
  void VtkTest::test_read_vector_attrib()
  {
    MeshImpl mesh;
    MsqPrintError err(cout);
    
    FILE* file = fopen( temp_file_name, "w+" );
    fputs( structured_3d_points_data, file );
    fputs( simple_vector_attrib, file );
    fclose( file );
    
    mesh.read_vtk( temp_file_name, err );
    remove( temp_file_name );
    CPPUNIT_ASSERT(!err);
    
    size_t num_vert, num_elem, num_uses;
    mesh.get_all_sizes( num_vert, num_elem, num_uses, err );
    CPPUNIT_ASSERT( !err );
    msq_std::vector<Mesh::VertexHandle> verts(num_vert);
    msq_std::vector<Mesh::ElementHandle> elems(num_elem);
    msq_std::vector<size_t> offsets(num_elem+1);
    msq_std::vector<size_t> uses(num_uses);
    mesh.get_all_mesh( &verts[0], verts.size(),
                       &elems[0], elems.size(),
                       &offsets[0], offsets.size(),
                       &uses[0], uses.size(), err );
    CPPUNIT_ASSERT( !err );
   
    void* th = mesh.tag_get( "hexvect", err );
    CPPUNIT_ASSERT( !err );

    msq_std::string name;
    Mesh::TagType type;
    unsigned tagsize;
    mesh.tag_properties( th, name, type, tagsize, err );
    CPPUNIT_ASSERT( !err && type == Mesh::DOUBLE && tagsize == 3 );
    
    double elem_data[24];
    mesh.tag_get_element_data( th, 8, &elems[0], elem_data, err );
    CPPUNIT_ASSERT( !err );
    
    for (int i = 0; i < 8; ++i)
      CPPUNIT_ASSERT( Vector3D( elem_data+3*i ) == Vector3D( i+1, i+1, i+1 ) );
  }
  

    // Test reading MeshImpl boundary-vertex bit
    // from Vtk scalar attribute.
  void VtkTest::test_read_fixed_attrib()
  {
    MeshImpl mesh;
    MsqPrintError err(cout);
    
    FILE* file = fopen( temp_file_name, "w+" );
    fputs( structured_3d_points_data, file );
    fputs( fixed_vertex_attrib, file );
    fclose( file );
    
    mesh.read_vtk( temp_file_name, err );
    remove( temp_file_name );
    CPPUNIT_ASSERT(!err);
    
    size_t vertex_count, element_count, use_count;
    mesh.get_all_sizes( vertex_count, element_count, use_count, err );
    CPPUNIT_ASSERT(!err && vertex_count == 27 && element_count == 8);
    CPPUNIT_ASSERT(use_count == 64);
    
    Mesh::VertexHandle vertices[27];
    Mesh::ElementHandle elements[8];
    size_t vertex_uses[64], offsets[9];
    mesh.get_all_mesh( vertices, 27, elements, 8, offsets, 9, vertex_uses, 64, err );
    CPPUNIT_ASSERT( !err );
    
    for (int i = 0; i < 27; ++i)
    {
      bool should_be_fixed = (i != 13);
      bool is_fixed ;
      mesh.vertices_are_on_boundary( vertices + i, &is_fixed, 1, err );
      CPPUNIT_ASSERT( !err );
      CPPUNIT_ASSERT( should_be_fixed == is_fixed );
    }
  }
 
    
    // Test writing VTK unstructured mesh
  void VtkTest::test_write()
  {
    MeshImpl mesh;
    MsqPrintError err(cout);
    
      // Create file containing unstructured mesh test case
    FILE* file = fopen( temp_file_name, "w+" );
    CPPUNIT_ASSERT(file);
    int rval = fputs( mixed_unstructured_data, file );
    fclose( file );
    if (rval == EOF) remove(temp_file_name);
    CPPUNIT_ASSERT(rval != EOF);
    
      // Read unstructured mesh file
    mesh.read_vtk( temp_file_name, err );
    remove(temp_file_name);
    CPPUNIT_ASSERT(!err);
    
      // Write unstructured mesh file back out
    mesh.write_vtk( temp_file_name, err );
    if (err)  remove( temp_file_name );
    CPPUNIT_ASSERT(!err);
    
      // Check if file contained expected mesh
    test_read_unstructured( temp_file_name );
  }
  
    
    // Test reading VTK unstructured mesh quadratic elements
  void VtkTest::test_read_quadratic()
  {
    FILE* file = fopen( temp_file_name, "w+" );
    CPPUNIT_ASSERT( file );
    int rval = fputs( quadratic_unstructured_data, file );
    fclose( file );
    if (rval == EOF) remove( temp_file_name );
    CPPUNIT_ASSERT( rval != EOF );

    test_read_quadratic( temp_file_name );
  }
  
  void VtkTest::test_read_quadratic( const char* filename )
  {
    MeshImpl mesh;
    MsqPrintError err(cout);
    
    mesh.read_vtk( filename, err );
    remove( temp_file_name );
    CPPUNIT_ASSERT(!err);
    
    size_t vertex_count, element_count, use_count;
    mesh.get_all_sizes( vertex_count, element_count, use_count, err );
    CPPUNIT_ASSERT(!err && vertex_count == 30 && element_count == 4);
    CPPUNIT_ASSERT(use_count == 44 );
    
    std::vector<Mesh::VertexHandle> verts(40);
    std::vector<Mesh::ElementHandle> elems(4);
    std::vector<size_t> uses(use_count), offsets(5);
    mesh.get_all_mesh( &verts[0], verts.size(),
                       &elems[0], elems.size(),
                       &offsets[0], offsets.size(),
                       &uses[0], uses.size(), err );
    CPPUNIT_ASSERT(!err);

    static const double hex_corners[] = 
     {  1.0, -1.0, -1.0, 
        1.0,  1.0, -1.0, 
       -1.0,  1.0, -1.0, 
       -1.0, -1.0, -1.0,
        1.0, -1.0,  1.0, 
        1.0,  1.0,  1.0, 
       -1.0,  1.0,  1.0, 
       -1.0, -1.0,  1.0 };
    static const double tet_corners[] = 
     {  1.0, -1.0, -1.0,
        1.0,  1.0, -1.0,
       -1.0,  0.0, -1.0,
        0.0,  0.0,  1.0 };
    static const unsigned hex_edges[] =
     { 0, 1, 
       1, 2, 
       2, 3,
       3, 0,
       0, 4,
       1, 5,
       2, 6,
       3, 7,
       4, 5,
       5, 6,
       6, 7,
       7, 4 };
    static const unsigned tet_edges[] = 
     { 0, 1,
       1, 2, 
       2, 0,
       0, 3,
       1, 3, 
       2, 3 };
    
    static const struct {
      EntityTopology topology;
      unsigned num_corners;
      unsigned num_edges;
      const double* corners;
      const unsigned* edges;
    } expected_elems[] = {
      { Mesquite::HEXAHEDRON,    8, 12, hex_corners, hex_edges },
      { Mesquite::TETRAHEDRON,   4,  6, tet_corners, tet_edges },
      { Mesquite::QUADRILATERAL, 4,  4, hex_corners, hex_edges },
      { Mesquite::TRIANGLE,      3,  3, tet_corners, tet_edges } };
    
    for (unsigned i = 0; i < 4; ++i)
    {
      EntityTopology topo = mesh.element_get_topology( elems[i], err );
      CPPUNIT_ASSERT( !err && expected_elems[i].topology == topo );
      
      size_t vtx_start = offsets[i];
      size_t vtx_end = offsets[i+1];
      CPPUNIT_ASSERT( (vtx_end - vtx_start) == 
         (expected_elems[i].num_corners + expected_elems[i].num_edges) );
      
      for (unsigned c = 0; c < expected_elems[i].num_corners; ++c)
      {
        Vector3D expected(expected_elems[i].corners + 3*c);
        MsqVertex have;
        mesh.vertices_get_coordinates( &verts[uses[vtx_start+c]], &have, 1, err );
        CPPUNIT_ASSERT(!err);
        expected -= have;
        CPPUNIT_ASSERT( expected.length() < DBL_EPSILON );
      }
      
      for (unsigned m = 0; m < expected_elems[i].num_edges; ++m)
      {
        unsigned start_idx = expected_elems[i].edges[2*m];
        unsigned end_idx = expected_elems[i].edges[2*m+1];
        Vector3D start( expected_elems[i].corners + 3*start_idx );
        Vector3D end( expected_elems[i].corners + 3*end_idx );
        Vector3D expected = 0.5 * (start + end);
        
        MsqVertex have;
        size_t vtx_idx = vtx_start + expected_elems[i].num_corners + m;
        mesh.vertices_get_coordinates( &verts[uses[vtx_idx]], &have, 1, err );
        CPPUNIT_ASSERT(!err);
        
        expected -= have;
        CPPUNIT_ASSERT( expected.length() < DBL_EPSILON );
      }
    }
  }
    
    // Test writing quadtratic elements
  void VtkTest::test_write_quadratic()
  {
    MeshImpl mesh;
    MsqPrintError err(cout);
    
      // Create file containing unstructured mesh test case
    FILE* file = fopen( temp_file_name, "w+" );
    CPPUNIT_ASSERT(file);
    int rval = fputs( quadratic_unstructured_data, file );
    fclose( file );
    if (rval == EOF) remove(temp_file_name);
    CPPUNIT_ASSERT(rval != EOF);
    
      // Read unstructured mesh file
    mesh.read_vtk( temp_file_name, err );
    remove(temp_file_name);
    CPPUNIT_ASSERT(!err);
    
      // Write unstructured mesh file back out
    mesh.write_vtk( temp_file_name, err );
    if (err)  remove( temp_file_name );
    CPPUNIT_ASSERT(!err);
    
      // Check if file contained expected mesh
    test_read_quadratic( temp_file_name );
  }

  void VtkTest::test_elements()
  {
    Mesquite::MsqPrintError err(cout);
    MeshImpl* mMesh = new MeshImpl;
    mMesh->read_vtk("../../meshFiles/2D/VTK/equil_tri2.vtk", err);
    CPPUNIT_ASSERT(!err);
    
      // Add mesh to a MeshSet.
    Mesquite::MeshSet mesh_set;
    mesh_set.add_mesh(mMesh, err); CPPUNIT_ASSERT(!err);
    
      // Retrieve a patch
    Mesquite::PatchData pd;
    Mesquite::PatchDataParameters pd_params;
    pd_params.set_patch_type(Mesquite::PatchData::ELEMENTS_ON_VERTEX_PATCH,
                             err, 1, 0);
    pd_params.add_culling_method(Mesquite::PatchData::NO_BOUNDARY_VTX);
    
    mesh_set.get_next_patch(pd, pd_params, err); CPPUNIT_ASSERT(!err);
//    mesh_set.write_gnuplot("toto", err); CPPUNIT_ASSERT(!err);
    
    int free_vtx = pd.num_free_vertices(err); CPPUNIT_ASSERT(!err);
//    std::cout << "nb of free vertices: " << free_vtx << std::endl;
    CPPUNIT_ASSERT( free_vtx == 1 );
    
    Mesquite::MsqMeshEntity* element_array =  pd.get_element_array(err); CPPUNIT_ASSERT(!err);
    size_t num_elements = pd.num_elements();
    CPPUNIT_ASSERT( num_elements == 6 );
    
    Mesquite::MsqVertex* vtx_array = pd.get_vertex_array(err); CPPUNIT_ASSERT(!err);
    size_t num_vertices = pd.num_vertices();
    CPPUNIT_ASSERT( num_vertices == 7 );
    
    CPPUNIT_ASSERT( tri_check_validity(element_array, num_elements, vtx_array, num_vertices) == 1 );
    
    mesh_set.get_next_patch(pd, pd_params, err); CPPUNIT_ASSERT(!err);
    
    element_array =  pd.get_element_array(err); CPPUNIT_ASSERT(!err);
    num_elements = pd.num_elements();
    CPPUNIT_ASSERT( num_elements == 6 );
    
    vtx_array = pd.get_vertex_array(err); CPPUNIT_ASSERT(!err);
    num_vertices = pd.num_vertices();
    CPPUNIT_ASSERT( num_vertices == 7 );
    
    CPPUNIT_ASSERT( tri_check_validity(element_array, num_elements, vtx_array, num_vertices) == 1 );
  }
  
  int VtkTest::tri_check_validity(Mesquite::MsqMeshEntity* element_array,
                         size_t num_elements,
                         Mesquite::MsqVertex* vtx_array,
                         size_t num_vertices)
  {
       
      /* check that the simplicial mesh is still valid, 
         based on right handedness. Returns a 1 or a 0 */
    int valid = 1;
    double dEps = 1.e-13;
    
    double x1, x2, x3, y1, y2, y3;// z1, z2, z3;
    std::vector<size_t> vertex_indices;
    
    for (size_t i=0;i<num_elements;i++)
    {
      element_array[i].get_vertex_indices(vertex_indices);
      
      x1 = vtx_array[vertex_indices[0]][0];
      y1 = vtx_array[vertex_indices[0]][1];
      x2 = vtx_array[vertex_indices[1]][0];
      y2 = vtx_array[vertex_indices[1]][1];
      x3 = vtx_array[vertex_indices[2]][0];
      y3 = vtx_array[vertex_indices[2]][1];
      
      double a = x2*y3 - x3*y2;
      double b = y2 - y3;
      double c = x3 - x2;
      
      double area = .5*(a+b*x1+c*y1);
      if (area < dEps) {
          //          printf("x1 y1 = %f %f\n",x1,y1);
          //          printf("x2 y3 = %f %f\n",x2,y2);
          //          printf("x3 y3 = %f %f\n",x3,y3);
          //          printf("area = %f\n",area);
        valid=0;
      }
    }
    
    return(valid);
  }
  
  int VtkTest::tet_validity_check(Mesquite::MsqMeshEntity* element_array,
                         size_t num_elements,
                         Mesquite::MsqVertex *vtx_array)
  {
    int valid = 1;
    double dEps = 1.e-13;
    double x1, y1, z1, x2, y2, z2, x3, y3, z3, x4, y4, z4;
    std::vector<size_t> vertex_indices;
    
    for (size_t i=0;i<num_elements;i++)
    {
      element_array[i].get_vertex_indices(vertex_indices);
      
      x1=vtx_array[vertex_indices[0]][0];
      y1=vtx_array[vertex_indices[0]][1];
      z1=vtx_array[vertex_indices[0]][2];
      
      x2=vtx_array[vertex_indices[1]][0];
      y2=vtx_array[vertex_indices[1]][1];
      z2=vtx_array[vertex_indices[1]][2];
      
      x3=vtx_array[vertex_indices[2]][0];
      y3=vtx_array[vertex_indices[2]][1];
      z3=vtx_array[vertex_indices[2]][2];
      
      x4=vtx_array[vertex_indices[3]][0];
      y4=vtx_array[vertex_indices[3]][1];
      z4=vtx_array[vertex_indices[3]][2];
      
      double dDX2 = x2 - x1;
      double dDX3 = x3 - x1;
      double dDX4 = x4 - x1;
      
      double dDY2 = y2 - y1;
      double dDY3 = y3 - y1;
      double dDY4 = y4 - y1;
      
      double dDZ2 = z2 - z1;
      double dDZ3 = z3 - z1;
      double dDZ4 = z4 - z1;
      
        /* dDet is proportional to the cell volume */
      double dDet = dDX2*dDY3*dDZ4 + dDX3*dDY4*dDZ2 + dDX4*dDY2*dDZ3
        - dDZ2*dDY3*dDX4 - dDZ3*dDY4*dDX2 - dDZ4*dDY2*dDX3 ;
      
        /* Compute a length scale based on edge lengths. */
      double dScale = ( sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2) +
                             (z1-z2)*(z1-z2)) +
                        sqrt((x1-x3)*(x1-x3) + (y1-y3)*(y1-y3) +
                             (z1-z3)*(z1-z3)) +
                        sqrt((x1-x4)*(x1-x4) + (y1-y4)*(y1-y4) +
                             (z1-z4)*(z1-z4)) +
                        sqrt((x2-x3)*(x2-x3) + (y2-y3)*(y2-y3) +
                             (z2-z3)*(z2-z3)) +
                        sqrt((x2-x4)*(x2-x4) + (y2-y4)*(y2-y4) +
                             (z2-z4)*(z2-z4)) +
                        sqrt((x3-x4)*(x3-x4) + (y3-y4)*(y3-y4) +
                             (z3-z4)*(z3-z4)) ) / 6.;
      
        /* Use the length scale to get a better idea if the tet is flat or
           just really small. */
      if (fabs(dScale) < dEps)
      {
        return(valid = 0);
      }
      else
      {
        dDet /= (dScale*dScale*dScale);
      }
      
      if (dDet > dEps)
      {
        valid = 1;
      }
      else if (dDet < -dEps)
      {
        valid = -1;
      }
      else
      {
        valid = 0;
      }
    }  // end for i=1,numElements
    
    return(valid);
  }


CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(VtkTest, "VtkTest");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(VtkTest, "Unit");

#else /* ifndef DEBUG */

int  main()
{
  FILE* file;
  
  file = fopen( "2d_structured_points.vtk", "w" );
  fputs( structured_2d_points_data, file );
  fclose( file );
  
  file = fopen( "3d_structured_points.vtk", "w" );
  fputs( structured_3d_points_data, file );
  fputs( fixed_vertex_attrib, file );
  fclose( file );
  
  file = fopen( "structured_grid.vtk", "w" );
  fputs( structured_grid_data, file );
  fputs( fixed_vertex_attrib, file );
  fclose( file );
  
  file = fopen( "rectilinear_grid.vtk", "w" );
  fputs( rectilinear_grid_data, file );
  fputs( fixed_vertex_attrib, file );
  fclose( file );
  
  file = fopen( "mixed_unstructured.vtk", "w" );
  fputs( mixed_unstructured_data, file );
  fclose( file );
  
  file = fopen( "quadratic.vtk", "w" );
  fputs( quadratic_unstructured_data, file );
  fclose( file );
  
  return 0;
}
#endif
