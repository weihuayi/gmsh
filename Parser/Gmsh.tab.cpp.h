typedef union {
  char    *c;
  int      i;
  unsigned int u;
  double   d;
  double   v[5];
  Shape    s;
  List_T  *l;
} YYSTYPE;
#define	tDOUBLE	257
#define	tSTRING	258
#define	tBIGSTR	259
#define	tEND	260
#define	tAFFECT	261
#define	tDOTS	262
#define	tPi	263
#define	tExp	264
#define	tLog	265
#define	tLog10	266
#define	tSqrt	267
#define	tSin	268
#define	tAsin	269
#define	tCos	270
#define	tAcos	271
#define	tTan	272
#define	tAtan	273
#define	tAtan2	274
#define	tSinh	275
#define	tCosh	276
#define	tTanh	277
#define	tFabs	278
#define	tFloor	279
#define	tCeil	280
#define	tFmod	281
#define	tModulo	282
#define	tHypot	283
#define	tPrintf	284
#define	tPoint	285
#define	tCircle	286
#define	tEllipsis	287
#define	tLine	288
#define	tSurface	289
#define	tSpline	290
#define	tVolume	291
#define	tCharacteristic	292
#define	tLength	293
#define	tParametric	294
#define	tElliptic	295
#define	tPlane	296
#define	tRuled	297
#define	tTransfinite	298
#define	tComplex	299
#define	tPhysical	300
#define	tUsing	301
#define	tBump	302
#define	tProgression	303
#define	tRotate	304
#define	tTranslate	305
#define	tSymmetry	306
#define	tDilate	307
#define	tExtrude	308
#define	tDuplicata	309
#define	tLoop	310
#define	tInclude	311
#define	tRecombine	312
#define	tDelete	313
#define	tCoherence	314
#define	tView	315
#define	tOffset	316
#define	tAttractor	317
#define	tLayers	318
#define	tScalarTetrahedron	319
#define	tVectorTetrahedron	320
#define	tTensorTetrahedron	321
#define	tScalarTriangle	322
#define	tVectorTriangle	323
#define	tTensorTriangle	324
#define	tScalarLine	325
#define	tVectorLine	326
#define	tTensorLine	327
#define	tScalarPoint	328
#define	tVectorPoint	329
#define	tTensorPoint	330
#define	tBSpline	331
#define	tNurbs	332
#define	tOrder	333
#define	tWith	334
#define	tBounds	335
#define	tKnots	336
#define	tColor	337
#define	tOptions	338
#define	tFor	339
#define	tEndFor	340
#define	tScript	341
#define	tExit	342
#define	tMerge	343
#define	tB_SPLINE_SURFACE_WITH_KNOTS	344
#define	tB_SPLINE_CURVE_WITH_KNOTS	345
#define	tCARTESIAN_POINT	346
#define	tTRUE	347
#define	tFALSE	348
#define	tUNSPECIFIED	349
#define	tU	350
#define	tV	351
#define	tEDGE_CURVE	352
#define	tVERTEX_POINT	353
#define	tORIENTED_EDGE	354
#define	tPLANE	355
#define	tFACE_OUTER_BOUND	356
#define	tEDGE_LOOP	357
#define	tADVANCED_FACE	358
#define	tVECTOR	359
#define	tDIRECTION	360
#define	tAXIS2_PLACEMENT_3D	361
#define	tISO	362
#define	tENDISO	363
#define	tENDSEC	364
#define	tDATA	365
#define	tHEADER	366
#define	tFILE_DESCRIPTION	367
#define	tFILE_SCHEMA	368
#define	tFILE_NAME	369
#define	tMANIFOLD_SOLID_BREP	370
#define	tCLOSED_SHELL	371
#define	tADVANCED_BREP_SHAPE_REPRESENTATION	372
#define	tFACE_BOUND	373
#define	tCYLINDRICAL_SURFACE	374
#define	tCONICAL_SURFACE	375
#define	tCIRCLE	376
#define	tTRIMMED_CURVE	377
#define	tGEOMETRIC_SET	378
#define	tCOMPOSITE_CURVE_SEGMENT	379
#define	tCONTINUOUS	380
#define	tCOMPOSITE_CURVE	381
#define	tTOROIDAL_SURFACE	382
#define	tPRODUCT_DEFINITION	383
#define	tPRODUCT_DEFINITION_SHAPE	384
#define	tSHAPE_DEFINITION_REPRESENTATION	385
#define	tELLIPSE	386
#define	tTrimmed	387
#define	tSolid	388
#define	tEndSolid	389
#define	tVertex	390
#define	tFacet	391
#define	tNormal	392
#define	tOuter	393
#define	tLoopSTL	394
#define	tEndLoop	395
#define	tEndFacet	396
#define	tAFFECTPLUS	397
#define	tAFFECTMINUS	398
#define	tAFFECTTIMES	399
#define	tAFFECTDIVIDE	400
#define	tAND	401
#define	tOR	402
#define	tNOTEQUAL	403
#define	tEQUAL	404
#define	tAPPROXEQUAL	405
#define	tLESSOREQUAL	406
#define	tGREATEROREQUAL	407
#define	tCROSSPRODUCT	408
#define	UNARYPREC	409
#define	tPLUSPLUS	410
#define	tMINUSMINUS	411


extern YYSTYPE yylval;
