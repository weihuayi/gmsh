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
#define	tDraw	285
#define	tSleep	286
#define	tPoint	287
#define	tCircle	288
#define	tEllipsis	289
#define	tLine	290
#define	tSurface	291
#define	tSpline	292
#define	tVolume	293
#define	tCharacteristic	294
#define	tLength	295
#define	tParametric	296
#define	tElliptic	297
#define	tPlane	298
#define	tRuled	299
#define	tTransfinite	300
#define	tComplex	301
#define	tPhysical	302
#define	tUsing	303
#define	tBump	304
#define	tProgression	305
#define	tRotate	306
#define	tTranslate	307
#define	tSymmetry	308
#define	tDilate	309
#define	tExtrude	310
#define	tDuplicata	311
#define	tLoop	312
#define	tInclude	313
#define	tRecombine	314
#define	tDelete	315
#define	tCoherence	316
#define	tView	317
#define	tAttractor	318
#define	tLayers	319
#define	tScalarTetrahedron	320
#define	tVectorTetrahedron	321
#define	tTensorTetrahedron	322
#define	tScalarTriangle	323
#define	tVectorTriangle	324
#define	tTensorTriangle	325
#define	tScalarLine	326
#define	tVectorLine	327
#define	tTensorLine	328
#define	tScalarPoint	329
#define	tVectorPoint	330
#define	tTensorPoint	331
#define	tBSpline	332
#define	tNurbs	333
#define	tOrder	334
#define	tWith	335
#define	tBounds	336
#define	tKnots	337
#define	tColor	338
#define	tFor	339
#define	tEndFor	340
#define	tScript	341
#define	tExit	342
#define	tMerge	343
#define	tReturn	344
#define	tCall	345
#define	tFunction	346
#define	tB_SPLINE_SURFACE_WITH_KNOTS	347
#define	tB_SPLINE_CURVE_WITH_KNOTS	348
#define	tCARTESIAN_POINT	349
#define	tTRUE	350
#define	tFALSE	351
#define	tUNSPECIFIED	352
#define	tU	353
#define	tV	354
#define	tEDGE_CURVE	355
#define	tVERTEX_POINT	356
#define	tORIENTED_EDGE	357
#define	tPLANE	358
#define	tFACE_OUTER_BOUND	359
#define	tEDGE_LOOP	360
#define	tADVANCED_FACE	361
#define	tVECTOR	362
#define	tDIRECTION	363
#define	tAXIS2_PLACEMENT_3D	364
#define	tISO	365
#define	tENDISO	366
#define	tENDSEC	367
#define	tDATA	368
#define	tHEADER	369
#define	tFILE_DESCRIPTION	370
#define	tFILE_SCHEMA	371
#define	tFILE_NAME	372
#define	tMANIFOLD_SOLID_BREP	373
#define	tCLOSED_SHELL	374
#define	tADVANCED_BREP_SHAPE_REPRESENTATION	375
#define	tFACE_BOUND	376
#define	tCYLINDRICAL_SURFACE	377
#define	tCONICAL_SURFACE	378
#define	tCIRCLE	379
#define	tTRIMMED_CURVE	380
#define	tGEOMETRIC_SET	381
#define	tCOMPOSITE_CURVE_SEGMENT	382
#define	tCONTINUOUS	383
#define	tCOMPOSITE_CURVE	384
#define	tTOROIDAL_SURFACE	385
#define	tPRODUCT_DEFINITION	386
#define	tPRODUCT_DEFINITION_SHAPE	387
#define	tSHAPE_DEFINITION_REPRESENTATION	388
#define	tELLIPSE	389
#define	tTrimmed	390
#define	tSolid	391
#define	tEndSolid	392
#define	tVertex	393
#define	tFacet	394
#define	tNormal	395
#define	tOuter	396
#define	tLoopSTL	397
#define	tEndLoop	398
#define	tEndFacet	399
#define	tAND	400
#define	tOR	401
#define	tNOTEQUAL	402
#define	tEQUAL	403
#define	tAPPROXEQUAL	404
#define	tAFFECTPLUS	405
#define	tAFFECTMINUS	406
#define	tAFFECTTIMES	407
#define	tAFFECTDIVIDE	408
#define	tLESSOREQUAL	409
#define	tGREATEROREQUAL	410
#define	tCROSSPRODUCT	411
#define	UNARYPREC	412
#define	tPLUSPLUS	413
#define	tMINUSMINUS	414


extern YYSTYPE yylval;
