// The MIT License (MIT)
//
// Copyright (c) 2013 Dan Ginsburg, Budirijanto Purnomo
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

//
// Book:      OpenGL(R) ES 3.0 Programming Guide, 2nd Edition
// Authors:   Dan Ginsburg, Budirijanto Purnomo, Dave Shreiner, Aaftab Munshi
// ISBN-10:   0-321-93388-5
// ISBN-13:   978-0-321-93388-1
// Publisher: Addison-Wesley Professional
// URLs:      http://www.opengles-book.com
//            http://my.safaribooksonline.com/book/animation-and-3d/9780133440133
//
// ESShapes.c
//
//    Utility functions for generating shapes
//

///
//  Includes
//
#include "esUtil.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>

///
// Defines
//
#define ES_PI  (3.14159265f)

//////////////////////////////////////////////////////////////////
//
//  Private Functions
//
//



//////////////////////////////////////////////////////////////////
//
//  Public Functions
//
//

/** 
 * Create a 3D coordination, a.k.a, 3 axis arrows.
 * 
 * @return Size of the `coordIndices'.
 */
int ESUTIL_API esGenCoordination(float scale,
                                 GLfloat **vertices,
                                 GLfloat **colors,
                                 GLuint  **indices)
{
    int numVertices = 30;
    int numIndices = 30;

    const GLfloat coordVerts[] = {
        -0.5f, 0.f,   0.f,      //!< x axis
        0.5f,  0.f,   0.f,
        0.4f,  0.1f,  0.f,      //!< arrow
        0.5f,  0.f,   0.f,
        0.4f, -0.1f,  0.f,
        0.5f,  0.f,   0.f,
        0.4f,  0.0f,  0.1f,
        0.5f,  0.f,   0.f,
        0.4f,  0.f,  -0.1f,
        0.5f,  0.f,   0.f,
        0.f,  -0.5f,  0.f,      //!< y axis
        0.f,   0.5f,  0.f,
        -0.1f, 0.4f,  0.f,      //!< arrow
        0.f,   0.5f,  0.f,
        0.1f,  0.4f,  0.f,
        0.f,   0.5f,  0.f,
        0.f,   0.4f, -0.1f,
        0.f,   0.5f,  0.f,
        0.f,   0.4f,  0.1f,
        0.f,   0.5f,  0.f,
        0.f,   0.f,  -0.5f,     //!< z axis
        0.f,   0.f,   0.5f,
        -0.1f, 0.f,   0.4f,      //!< arrow
        0.f,   0.f,   0.5f,
        0.1f,  0.f,   0.4f,
        0.f,   0.f,   0.5f,
        0.f,  -0.1f,  0.4f,
        0.f,   0.f,   0.5f,
        0.f,   0.1f,  0.4f,
        0.f,   0.f,   0.5f
    };
    GLfloat coordColors[] = {
        1.0f, 0.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 0.0f, 1.0f,
        0.0f, 1.0f, 0.0f, 1.0f,
        0.0f, 1.0f, 0.0f, 1.0f,
        0.0f, 1.0f, 0.0f, 1.0f,
        0.0f, 1.0f, 0.0f, 1.0f,
        0.0f, 1.0f, 0.0f, 1.0f,
        0.0f, 1.0f, 0.0f, 1.0f,
        0.0f, 1.0f, 0.0f, 1.0f,
        0.0f, 1.0f, 0.0f, 1.0f,
        0.0f, 1.0f, 0.0f, 1.0f,
        0.0f, 1.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f, 1.0f,
        0.0f, 0.0f, 1.0f, 1.0f,
        0.0f, 0.0f, 1.0f, 1.0f,
        0.0f, 0.0f, 1.0f, 1.0f,
        0.0f, 0.0f, 1.0f, 1.0f,
        0.0f, 0.0f, 1.0f, 1.0f,
        0.0f, 0.0f, 1.0f, 1.0f,
        0.0f, 0.0f, 1.0f, 1.0f,
        0.0f, 0.0f, 1.0f, 1.0f,
        0.0f, 0.0f, 1.0f, 1.0f
    };

    if (vertices != NULL) {
        *vertices = malloc(sizeof(GLfloat) * 3 * numVertices);
        memcpy(*vertices, coordVerts, sizeof(coordVerts));

        for (int i = 0; i < numVertices * 3; i++){
            (*vertices)[i] *= scale;
        }
    }

    if (colors != NULL) {
        *colors = malloc(sizeof(GLfloat) * 4 * numVertices);
        memcpy(*colors, coordColors, sizeof(coordColors));
    }

    // Generate the indices
    if (indices != NULL) {
        GLuint coordIndices[] = {
            0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
            10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
            20, 21, 22, 23, 24, 25, 26, 27, 28, 29
        };

        *indices = malloc(sizeof(GLuint) * numIndices);
        memcpy(*indices, coordIndices, sizeof(coordIndices));
    }

    return numIndices;
}

/** 
 * Create a triangle.
 *
 * @return sizeof(triIndices)
 */
int ESUTIL_API esGenTriangle(float scale,
                             GLfloat **vertices,
                             GLfloat **normals,
                             GLfloat **texCoords,
                             GLuint  **indices)
{
    int numVertices = 3;
    int numIndices = 3;

    const GLfloat triVerts[] = {
       0.0f,  0.5f, 0.0f,
       -0.5f, -0.5f, 0.0f,
       0.5f, -0.5f, 0.0f
    };
    const GLfloat triNormals[] = {
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,
    };
    const GLfloat triTex[] = {
        0.5f, 0.25f,
        0.25f, 0.75f,
        0.75f, 0.75f
    };

    if (vertices != NULL) {
        *vertices = malloc(sizeof(GLfloat) * 3 * numVertices);
        memcpy(*vertices, triVerts, sizeof(triVerts));

        for (int i = 0; i < numVertices * 3; i++){
            (*vertices)[i] *= scale;
        }
    }

    if (normals != NULL) {
        *normals = malloc(sizeof(GLfloat) * 3 * numVertices);
        memcpy(*normals, triNormals, sizeof(triNormals));
    }

    if (texCoords != NULL) {
        *texCoords = malloc(sizeof(GLfloat) * 2 * numVertices);
        memcpy(*texCoords, triTex, sizeof(triTex));
    }

    // Generate the indices
    if (indices != NULL) {
        GLuint triIndices[] = {0, 2, 1};
        *indices = malloc(sizeof(GLuint) * numIndices);
        memcpy(*indices, triIndices, sizeof(triIndices));
    }

    return numIndices;
}

//
/// \brief Generates geometry for a sphere.  Allocates memory for the vertex data and stores
///        the results in the arrays.  Generate index list for a TRIANGLE_STRIP
/// \param numSlices The number of slices in the sphere
/// \param vertices If not NULL, will contain array of float3 positions
/// \param normals If not NULL, will contain array of float3 normals
/// \param texCoords If not NULL, will contain array of float2 texCoords
/// \param indices If not NULL, will contain the array of indices for the triangle strip
/// \return The number of indices required for rendering the buffers (the number of indices stored in the indices array
///         if it is not NULL ) as a GL_TRIANGLE_STRIP
//
int ESUTIL_API esGenSphere ( int numSlices, float radius, GLfloat **vertices, GLfloat **normals,
                             GLfloat **texCoords, GLuint **indices )
{
   int i;
   int j;
   int numParallels = numSlices / 2;
   int numVertices = ( numParallels + 1 ) * ( numSlices + 1 );
   int numIndices = numParallels * numSlices * 6;
   float angleStep = ( 2.0f * ES_PI ) / ( ( float ) numSlices );

   // Allocate memory for buffers
   if ( vertices != NULL )
   {
      *vertices = malloc ( sizeof ( GLfloat ) * 3 * numVertices );
   }

   if ( normals != NULL )
   {
      *normals = malloc ( sizeof ( GLfloat ) * 3 * numVertices );
   }

   if ( texCoords != NULL )
   {
      *texCoords = malloc ( sizeof ( GLfloat ) * 2 * numVertices );
   }

   if ( indices != NULL )
   {
      *indices = malloc ( sizeof ( GLuint ) * numIndices );
   }

   for ( i = 0; i < numParallels + 1; i++ )
   {
      for ( j = 0; j < numSlices + 1; j++ )
      {
         int vertex = ( i * ( numSlices + 1 ) + j ) * 3;

         if ( vertices )
         {
            ( *vertices ) [vertex + 0] = radius * sinf ( angleStep * ( float ) i ) *
                                         sinf ( angleStep * ( float ) j );
            ( *vertices ) [vertex + 1] = radius * cosf ( angleStep * ( float ) i );
            ( *vertices ) [vertex + 2] = radius * sinf ( angleStep * ( float ) i ) *
                                         cosf ( angleStep * ( float ) j );
         }

         if ( normals )
         {
            ( *normals ) [vertex + 0] = ( *vertices ) [vertex + 0] / radius;
            ( *normals ) [vertex + 1] = ( *vertices ) [vertex + 1] / radius;
            ( *normals ) [vertex + 2] = ( *vertices ) [vertex + 2] / radius;
         }

         if ( texCoords )
         {
            int texIndex = ( i * ( numSlices + 1 ) + j ) * 2;
            ( *texCoords ) [texIndex + 0] = ( float ) j / ( float ) numSlices;
            ( *texCoords ) [texIndex + 1] = ( 1.0f - ( float ) i ) / ( float ) ( numParallels - 1 );
         }
      }
   }

   // Generate the indices
   if ( indices != NULL )
   {
      GLuint *indexBuf = ( *indices );

      for ( i = 0; i < numParallels ; i++ )
      {
         for ( j = 0; j < numSlices; j++ )
         {
            *indexBuf++  = i * ( numSlices + 1 ) + j;
            *indexBuf++ = ( i + 1 ) * ( numSlices + 1 ) + j;
            *indexBuf++ = ( i + 1 ) * ( numSlices + 1 ) + ( j + 1 );

            *indexBuf++ = i * ( numSlices + 1 ) + j;
            *indexBuf++ = ( i + 1 ) * ( numSlices + 1 ) + ( j + 1 );
            *indexBuf++ = i * ( numSlices + 1 ) + ( j + 1 );
         }
      }
   }

   return numIndices;
}

//
/// \brief Generates geometry for a cube.  Allocates memory for the vertex data and stores
///        the results in the arrays.  Generate index list for a TRIANGLES
/// \param scale The size of the cube, use 1.0 for a unit cube.
/// \param vertices If not NULL, will contain array of float3 positions
/// \param normals If not NULL, will contain array of float3 normals
/// \param texCoords If not NULL, will contain array of float2 texCoords
/// \param indices If not NULL, will contain the array of indices for the triangle strip
/// \return The number of indices required for rendering the buffers (the number of indices stored in the indices array
///         if it is not NULL ) as a GL_TRIANGLE_STRIP
//
int ESUTIL_API esGenCube ( float scale, GLfloat **vertices, GLfloat **normals,
                           GLfloat **texCoords, GLuint **indices )
{
   int i;
   int numVertices = 24;
   int numIndices = 36;

   GLfloat cubeVerts[] =
   {
      -0.5f, -0.5f, -0.5f,
      -0.5f, -0.5f,  0.5f,
      0.5f, -0.5f,  0.5f,
      0.5f, -0.5f, -0.5f,
      -0.5f,  0.5f, -0.5f,
      -0.5f,  0.5f,  0.5f,
      0.5f,  0.5f,  0.5f,
      0.5f,  0.5f, -0.5f,
      -0.5f, -0.5f, -0.5f,
      -0.5f,  0.5f, -0.5f,
      0.5f,  0.5f, -0.5f,
      0.5f, -0.5f, -0.5f,
      -0.5f, -0.5f, 0.5f,
      -0.5f,  0.5f, 0.5f,
      0.5f,  0.5f, 0.5f,
      0.5f, -0.5f, 0.5f,
      -0.5f, -0.5f, -0.5f,
      -0.5f, -0.5f,  0.5f,
      -0.5f,  0.5f,  0.5f,
      -0.5f,  0.5f, -0.5f,
      0.5f, -0.5f, -0.5f,
      0.5f, -0.5f,  0.5f,
      0.5f,  0.5f,  0.5f,
      0.5f,  0.5f, -0.5f,
   };

   GLfloat cubeNormals[] =
   {
      0.0f, -1.0f, 0.0f,
      0.0f, -1.0f, 0.0f,
      0.0f, -1.0f, 0.0f,
      0.0f, -1.0f, 0.0f,
      0.0f, 1.0f, 0.0f,
      0.0f, 1.0f, 0.0f,
      0.0f, 1.0f, 0.0f,
      0.0f, 1.0f, 0.0f,
      0.0f, 0.0f, -1.0f,
      0.0f, 0.0f, -1.0f,
      0.0f, 0.0f, -1.0f,
      0.0f, 0.0f, -1.0f,
      0.0f, 0.0f, 1.0f,
      0.0f, 0.0f, 1.0f,
      0.0f, 0.0f, 1.0f,
      0.0f, 0.0f, 1.0f,
      -1.0f, 0.0f, 0.0f,
      -1.0f, 0.0f, 0.0f,
      -1.0f, 0.0f, 0.0f,
      -1.0f, 0.0f, 0.0f,
      1.0f, 0.0f, 0.0f,
      1.0f, 0.0f, 0.0f,
      1.0f, 0.0f, 0.0f,
      1.0f, 0.0f, 0.0f,
   };

   GLfloat cubeTex[] =
   {
      0.0f, 0.0f,
      0.0f, 1.0f,
      1.0f, 1.0f,
      1.0f, 0.0f,
      1.0f, 0.0f,
      1.0f, 1.0f,
      0.0f, 1.0f,
      0.0f, 0.0f,
      0.0f, 0.0f,
      0.0f, 1.0f,
      1.0f, 1.0f,
      1.0f, 0.0f,
      0.0f, 0.0f,
      0.0f, 1.0f,
      1.0f, 1.0f,
      1.0f, 0.0f,
      0.0f, 0.0f,
      0.0f, 1.0f,
      1.0f, 1.0f,
      1.0f, 0.0f,
      0.0f, 0.0f,
      0.0f, 1.0f,
      1.0f, 1.0f,
      1.0f, 0.0f,
   };

   // Allocate memory for buffers
   if ( vertices != NULL )
   {
      *vertices = malloc ( sizeof ( GLfloat ) * 3 * numVertices );
      memcpy ( *vertices, cubeVerts, sizeof ( cubeVerts ) );

      for ( i = 0; i < numVertices * 3; i++ )
      {
         ( *vertices ) [i] *= scale;
      }
   }

   if ( normals != NULL )
   {
      *normals = malloc ( sizeof ( GLfloat ) * 3 * numVertices );
      memcpy ( *normals, cubeNormals, sizeof ( cubeNormals ) );
   }

   if ( texCoords != NULL )
   {
      *texCoords = malloc ( sizeof ( GLfloat ) * 2 * numVertices );
      memcpy ( *texCoords, cubeTex, sizeof ( cubeTex ) ) ;
   }


   // Generate the indices
   if ( indices != NULL )
   {
      GLuint cubeIndices[] =
      {
         0, 2, 1,
         0, 3, 2,
         4, 5, 6,
         4, 6, 7,
         8, 9, 10,
         8, 10, 11,
         12, 15, 14,
         12, 14, 13,
         16, 17, 18,
         16, 18, 19,
         20, 23, 22,
         20, 22, 21
      };

      *indices = malloc ( sizeof ( GLuint ) * numIndices );
      memcpy ( *indices, cubeIndices, sizeof ( cubeIndices ) );
   }

   return numIndices;
}

//
/// \brief Generates a square grid consisting of triangles.  Allocates memory for the vertex data and stores
///        the results in the arrays.  Generate index list as TRIANGLES.
/// \param size create a grid of size by size (number of triangles = (size-1)*(size-1)*2)
/// \param vertices If not NULL, will contain array of float3 positions
/// \param indices If not NULL, will contain the array of indices for the triangle strip
/// \return The number of indices required for rendering the buffers (the number of indices stored in the indices array
///         if it is not NULL ) as a GL_TRIANGLES
//
int ESUTIL_API esGenSquareGrid ( int size, GLfloat **vertices, GLuint **indices )
{
   int i, j;
   int numIndices = ( size - 1 ) * ( size - 1 ) * 2 * 3;

   // Allocate memory for buffers
   if ( vertices != NULL )
   {
      int numVertices = size * size;
      float stepSize = ( float ) size - 1;
      *vertices = malloc ( sizeof ( GLfloat ) * 3 * numVertices );

      for ( i = 0; i < size; ++i ) // row
      {
         for ( j = 0; j < size; ++j ) // column
         {
            ( *vertices ) [ 3 * ( j + i * size )     ] = i / stepSize;
            ( *vertices ) [ 3 * ( j + i * size ) + 1 ] = j / stepSize;
            ( *vertices ) [ 3 * ( j + i * size ) + 2 ] = 0.0f;
         }
      }
   }

   // Generate the indices
   if ( indices != NULL )
   {
      *indices = malloc ( sizeof ( GLuint ) * numIndices );

      for ( i = 0; i < size - 1; ++i )
      {
         for ( j = 0; j < size - 1; ++j )
         {
            // two triangles per quad
            ( *indices ) [ 6 * ( j + i * ( size - 1 ) )     ] = j + ( i )   * ( size )    ;
            ( *indices ) [ 6 * ( j + i * ( size - 1 ) ) + 1 ] = j + ( i )   * ( size ) + 1;
            ( *indices ) [ 6 * ( j + i * ( size - 1 ) ) + 2 ] = j + ( i + 1 ) * ( size ) + 1;

            ( *indices ) [ 6 * ( j + i * ( size - 1 ) ) + 3 ] = j + ( i )   * ( size )    ;
            ( *indices ) [ 6 * ( j + i * ( size - 1 ) ) + 4 ] = j + ( i + 1 ) * ( size ) + 1;
            ( *indices ) [ 6 * ( j + i * ( size - 1 ) ) + 5 ] = j + ( i + 1 ) * ( size )    ;
         }
      }
   }

   return numIndices;
}
