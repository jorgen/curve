/*
 * SGI FREE SOFTWARE LICENSE B (Version 2.0, Sept. 18, 2008)
 * Copyright (C) 1991-2000 Silicon Graphics, Inc. All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice including the dates of first publication and
 * either this permission notice or a reference to
 * http://oss.sgi.com/projects/FreeB/
 * shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * SILICON GRAPHICS, INC. BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
 * OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * Except as contained in this notice, the name of Silicon Graphics, Inc.
 * shall not be used in advertising or otherwise to promote the sale, use or
 * other dealings in this Software without prior written authorization from
 * Silicon Graphics, Inc.
 */
/*
** Author: Eric Veach, July 1994.
**
*/

#ifndef __tess_h_
#define __tess_h_

#include <setjmp.h>
#include "mesh.h"
#include "dict.h"
#include "priorityq.h"

#define TESSAPIENTRY

typedef int TessEnum;

typedef void (TESSAPIENTRY *TessFuncptr)(void);

#define TESS_INVALID_ENUM        100900
#define TESS_INVALID_VALUE       100901
#define TESS_OUT_OF_MEMORY       100902
#define TESS_INCOMPATIBLE_GL_VERSION     100903

#define TESS_MAX_COORD              1.0e150

/* BeginMode */
#define TESS_POINTS                         0x0000
#define TESS_LINES                          0x0001
#define TESS_LINE_LOOP                      0x0002
#define TESS_LINE_STRIP                     0x0003
#define TESS_TRIANGLES                      0x0004
#define TESS_TRIANGLE_STRIP                 0x0005
#define TESS_TRIANGLE_FAN                   0x0006
#define TESS_QUADS                          0x0007
#define TESS_QUAD_STRIP                     0x0008
#define TESS_POLYGON                        0x0009

/* TessProperty */
#define TESS_WINDING_RULE           100140
#define TESS_BOUNDARY_ONLY          100141
#define TESS_TOLERANCE              100142

/* TessWinding */
#define TESS_WINDING_ODD            100130
#define TESS_WINDING_NONZERO        100131
#define TESS_WINDING_POSITIVE       100132
#define TESS_WINDING_NEGATIVE       100133
#define TESS_WINDING_ABS_GEQ_TWO    100134

/* TessError */
#define TESS_ERROR1     100151
#define TESS_ERROR2     100152
#define TESS_ERROR3     100153
#define TESS_ERROR4     100154
#define TESS_ERROR5     100155
#define TESS_ERROR6     100156
#define TESS_ERROR7     100157
#define TESS_ERROR8     100158

#define TESS_MISSING_BEGIN_POLYGON  TESS_ERROR1
#define TESS_MISSING_BEGIN_CONTOUR  TESS_ERROR2
#define TESS_MISSING_END_POLYGON    TESS_ERROR3
#define TESS_MISSING_END_CONTOUR    TESS_ERROR4
#define TESS_COORD_TOO_LARGE        TESS_ERROR5
#define TESS_NEED_COMBINE_CALLBACK  TESS_ERROR6

/* TessCallback */
#define TESS_BEGIN          100100  /* void (CALLBACK*)(GLenum    type)  */
#define TESS_VERTEX         100101  /* void (CALLBACK*)(void      *data) */
#define TESS_END            100102  /* void (CALLBACK*)(void)            */
#define TESS_ERROR          100103  /* void (CALLBACK*)(GLenum    errno) */
#define TESS_EDGE_FLAG      100104  /* void (CALLBACK*)(GLboolean boundaryEdge)  */
#define TESS_COMBINE        100105  /* void (CALLBACK*)(GLdouble  coords[3],
                                                        void      *data[4],
                                                        GLfloat   weight[4],
                                                        void      **dataOut)     */
#define TESS_BEGIN_DATA     100106  /* void (CALLBACK*)(GLenum    type,  
                                                        void      *polygon_data) */
#define TESS_VERTEX_DATA    100107  /* void (CALLBACK*)(void      *data, 
                                                        void      *polygon_data) */
#define TESS_END_DATA       100108  /* void (CALLBACK*)(void      *polygon_data) */
#define TESS_ERROR_DATA     100109  /* void (CALLBACK*)(GLenum    errno, 
                                                        void      *polygon_data) */
#define TESS_EDGE_FLAG_DATA 100110  /* void (CALLBACK*)(GLboolean boundaryEdge,
                                                        void      *polygon_data) */
#define TESS_COMBINE_DATA   100111  /* void (CALLBACK*)(GLdouble  coords[3],
                                                            void      *data[4],
                                                            GLfloat   weight[4],
                                                            void      **dataOut,
                                                            void      *polygon_data) */
/* The begin/end calls must be properly nested.  We keep track of
 * the current state to enforce the ordering.
 */
enum TessState { T_DORMANT, T_IN_POLYGON, T_IN_CONTOUR };

/* We cache vertex data for single-contour polygons so that we can
 * try a quick-and-dirty decomposition first.
 */
#define TESS_MAX_CACHE	100

typedef struct CachedVertex {
  double	coords[3];
  void		*data;
} CachedVertex;

typedef struct TessTesselator TessTesselator;
struct TessTesselator {

  /*** state needed for collecting the input data ***/

  enum TessState state;		/* what begin/end calls have we seen? */

  TessHalfEdge	*lastEdge;	/* lastEdge->Org is the most recent vertex */
  TessMesh	*mesh;		/* stores the input contours, and eventually
                                   the tessellation itself */

  void		(TESSAPIENTRY *callError)( TessEnum errnum );

  /*** state needed for projecting onto the sweep plane ***/

  double	normal[3];	/* user-specified normal (if provided) */
  double	sUnit[3];	/* unit vector in s-direction (debugging) */
  double	tUnit[3];	/* unit vector in t-direction (debugging) */

  /*** state needed for the line sweep ***/

  double	relTolerance;	/* tolerance for merging features */
  TessEnum	windingRule;	/* rule for determining polygon interior */
  bool	fatalError;	/* fatal error: needed combine callback */

  Dict		*dict;		/* edge dictionary for sweep line */
  PriorityQ	*pq;		/* priority queue of vertex events */
  TessVertex	*event;		/* current sweep event being processed */

  void		(TESSAPIENTRY *callCombine)( double coords[3], void *data[4],
			        float weight[4], void **outData );

  /*** state needed for rendering callbacks (see render.c) ***/

  bool	flagBoundary;	/* mark boundary edges (use EdgeFlag) */
  bool	boundaryOnly;	/* Extract contours, not triangles */
  TessFace	*lonelyTriList;
    /* list of triangles which could not be rendered as strips or fans */

  void		(TESSAPIENTRY *callBegin)( TessEnum type );
  void		(TESSAPIENTRY *callEdgeFlag)( bool boundaryEdge );
  void		(TESSAPIENTRY *callVertex)( void *data );
  void		(TESSAPIENTRY *callEnd)( void );
  void		(TESSAPIENTRY *callMesh)( TessMesh *mesh );


  /*** state needed to cache single-contour polygons for renderCache() */

  bool	emptyCache;		/* empty cache on next vertex() call */
  int		cacheCount;		/* number of cached vertices */
  CachedVertex	cache[TESS_MAX_CACHE];	/* the vertex data */

  /*** rendering callbacks that also pass polygon data  ***/ 
  void		(TESSAPIENTRY *callBeginData)( TessEnum type, void *polygonData );
  void		(TESSAPIENTRY *callEdgeFlagData)( bool boundaryEdge, 
				     void *polygonData );
  void		(TESSAPIENTRY *callVertexData)( void *data, void *polygonData );
  void		(TESSAPIENTRY *callEndData)( void *polygonData );
  void		(TESSAPIENTRY *callErrorData)( TessEnum errnum, void *polygonData );
  void		(TESSAPIENTRY *callCombineData)( double coords[3], void *data[4],
				    float weight[4], void **outData,
				    void *polygonData );

  jmp_buf env;			/* place to jump to when memAllocs fail */

  void *polygonData;		/* client data for current polygon */
};

TessTesselator* TESSAPIENTRY tessNewTess (void);
void TESSAPIENTRY tessDeleteTess (TessTesselator* tess);

void TESSAPIENTRY tessCallback (TessTesselator* tess, TessEnum which, TessFuncptr CallBackFunc);
void TESSAPIENTRY tessNormal (TessTesselator* tess, double valueX, double valueY, double valueZ);
void TESSAPIENTRY tessVertex (TessTesselator *tess, const double coords[3], void* data);
void TESSAPIENTRY tessProperty (TessTesselator* tess, TessEnum which, double data);
void TESSAPIENTRY tessGetTessProperty (TessTesselator* tess, TessEnum which, double* data);

void TESSAPIENTRY tessBeginPolygon (TessTesselator* tess, void* data);
void TESSAPIENTRY tessEndPolygon (TessTesselator* tess);

void TESSAPIENTRY tessBeginContour (TessTesselator* tess);
void TESSAPIENTRY tessEndContour (TessTesselator* tess);

void TESSAPIENTRY _tess_noBeginData( TessEnum type, void *polygonData );
void TESSAPIENTRY _tess_noEdgeFlagData( bool boundaryEdge, void *polygonData );
void TESSAPIENTRY _tess_noVertexData( void *data, void *polygonData );
void TESSAPIENTRY _tess_noEndData( void *polygonData );
void TESSAPIENTRY _tess_noErrorData( TessEnum errnum, void *polygonData );
void TESSAPIENTRY _tess_noCombineData( double coords[3], void *data[4],
			 float weight[4], void **outData,
			 void *polygonData );

#define CALL_BEGIN_OR_BEGIN_DATA(a) \
   if (tess->callBeginData != &_tess_noBeginData) \
      (*tess->callBeginData)((a),tess->polygonData); \
   else (*tess->callBegin)((a));

#define CALL_VERTEX_OR_VERTEX_DATA(a) \
   if (tess->callVertexData != &_tess_noVertexData) \
      (*tess->callVertexData)((a),tess->polygonData); \
   else (*tess->callVertex)((a));

#define CALL_EDGE_FLAG_OR_EDGE_FLAG_DATA(a) \
   if (tess->callEdgeFlagData != &_tess_noEdgeFlagData) \
      (*tess->callEdgeFlagData)((a),tess->polygonData); \
   else (*tess->callEdgeFlag)((a));

#define CALL_END_OR_END_DATA() \
   if (tess->callEndData != &_tess_noEndData) \
      (*tess->callEndData)(tess->polygonData); \
   else (*tess->callEnd)();

#define CALL_COMBINE_OR_COMBINE_DATA(a,b,c,d) \
   if (tess->callCombineData != &_tess_noCombineData) \
      (*tess->callCombineData)((a),(b),(c),(d),tess->polygonData); \
   else (*tess->callCombine)((a),(b),(c),(d));

#define CALL_ERROR_OR_ERROR_DATA(a) \
   if (tess->callErrorData != &_tess_noErrorData) \
      (*tess->callErrorData)((a),tess->polygonData); \
   else (*tess->callError)((a));

#endif
