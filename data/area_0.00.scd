(Material matRED
	(color 0.75 0.25 0.25 )
	(ma 0.5 ) (ml 0.6 ) (ms 0.1 ) (msp 150 ) (msm 1 ) (mt 0 ) (mtn 1 ))
(Material matBLUE
	(color 0.25 0.25 0.75 )
	(ma 0.5 ) (ml 0.6 ) (ms 0.1 ) (msp 20 ) (msm 1 ) (mt 0 ) (mtn 1 ))
(Material matWHITE
	(color 0.75 0.75 0.75 )
	(ma 0.5 ) (ml 0.6 ) (ms 0.1 ) (msp 40 ) (msm 1 ) (mt 0 ) (mtn 1 ))
(Light amb (type 0 ) (color 0.2 0.2 0.2 ) )
(Light are (type 2 ) (color 20.8 20.8 20.8 )  (falloff 1 )  (deaddistance 0.1 ))
(Camera cam
	(perspective 1 )
	(l -1 ) (r 1 ) (b -1 ) (t 1 ) (n 3 )) #no far plane
(Sphere spWALL (radius 100000.0 ) (material matWHITE ))
(Sphere spLEFT (radius 100000.0 ) (material matBLUE ))
(Sphere spRIGHT (radius 100000.0 ) (material matRED ))

(Material helixMat23
	(color 0.575 0.425 0.894400281756 )
	(ma 0.4 ) (ml 0.8 ) (ms 0.5 ) (msp 50 ) (msm 0.5 ) (mt 0 ) (mtn 1 ))
(Sphere sp_23 (radius 1.5 ) (material helixMat23 ))
(Material helixMat24
	(color 0.6 0.4 0.146916859426 )
	(ma 0.4 ) (ml 0.8 ) (ms 0.5 ) (msp 50 ) (msm 0.5 ) (mt 0 ) (mtn 1 ))
(Sphere sp_24 (radius 1.5 ) (material helixMat24 ))
(Material helixMat25
	(color 0.625 0.375 0.273021618904 )
	(ma 0.4 ) (ml 0.8 ) (ms 0.5 ) (msp 50 ) (msm 0.5 ) (mt 0 ) (mtn 1 ))
(Sphere sp_25 (radius 1.5 ) (material helixMat25 ))
(Material helixMat26
	(color 0.65 0.35 0.884016177378 )
	(ma 0.4 ) (ml 0.8 ) (ms 0.5 ) (msp 50 ) (msm 0.5 ) (mt 0 ) (mtn 1 ))
(Sphere sp_26 (radius 1.5 ) (material helixMat26 ))
(Material helixMat27
	(color 0.675 0.325 0.946384528027 )
	(ma 0.4 ) (ml 0.8 ) (ms 0.5 ) (msp 50 ) (msm 0.5 ) (mt 0 ) (mtn 1 ))
(Sphere sp_27 (radius 1.5 ) (material helixMat27 ))
(Material helixMat28
	(color 0.7 0.3 0.326229622424 )
	(ma 0.4 ) (ml 0.8 ) (ms 0.5 ) (msp 50 ) (msm 0.5 ) (mt 0 ) (mtn 1 ))
(Sphere sp_28 (radius 1.5 ) (material helixMat28 ))
(Material helixMat29
	(color 0.725 0.275 0.827181024652 )
	(ma 0.4 ) (ml 0.8 ) (ms 0.5 ) (msp 50 ) (msm 0.5 ) (mt 0 ) (mtn 1 ))
(Sphere sp_29 (radius 1.5 ) (material helixMat29 ))
(Material helixMat30
	(color 0.75 0.25 0.195082364552 )
	(ma 0.4 ) (ml 0.8 ) (ms 0.5 ) (msp 50 ) (msm 0.5 ) (mt 0 ) (mtn 1 ))
(Sphere sp_30 (radius 1.5 ) (material helixMat30 ))
(Material helixMat31
	(color 0.775 0.225 0.710957939404 )
	(ma 0.4 ) (ml 0.8 ) (ms 0.5 ) (msp 50 ) (msm 0.5 ) (mt 0 ) (mtn 1 ))
(Sphere sp_31 (radius 1.5 ) (material helixMat31 ))

(G awesome
	(I cam1 cam )

	(I amb1 amb )
	(I are_I are (T 2 2 -45 ) )

	(I s_23 sp_23 (T 4.85410196625 3.52671151375 -54 ) )
	(I s_24 sp_24 (T 5.70633909777 1.85410196625 -52 ) )
	(I s_25 sp_25 (T 6.0 1.83697019872e-15 -50 ) )
	(I s_26 sp_26 (T 5.70633909777 -1.85410196625 -48 ) )
	(I s_27 sp_27 (T 4.85410196625 -3.52671151375 -46 ) )
	(I s_28 sp_28 (T 3.52671151375 -4.85410196625 -44 ) )
	(I s_29 sp_29 (T 1.85410196625 -5.70633909777 -42 ) )
	(I s_30 sp_30 (T 2.20436423847e-15 -6.0 -40 ) )
	(I s_31 sp_31 (T -1.85410196625 -5.70633909777 -38 ) )
	(I spB_I spWALL (T 0 -100009.0 -3 ) )
	(I spT_I spWALL (T 0 100009.0 -3 ) )
	(I spF_I spWALL (T 0 0 -100100.0 ) )
	(I spN_I spWALL (T 0 0 100000.0 ) )
	(I spL_I spLEFT (T -100009.0 0 -3 ) )
	(I spR_I spRIGHT (T 100009.0 0 -3 ) )
)
(Render awesome )
