#pragma once


typedef enum
{
	DM_CONNECT,				// message id for connection to server 
	DM_SHUTDOWN,			// command to ask client shutdown itself, from server to client
	DM_SETBASEINFO,			// command to setup system base info, server => client

	DM_NEWPROJECT,			// command to ask client to new a project, server => client
	DM_LOADPROJECT,			// command to ask client to load a project, server => client
	DM_DELETEPROJECT,		// command to ask client to delete a project, server => client
	DM_CHANGE_PRJNAME,		// command to ask client to change project name, server => client
	DM_CHANGE_PRJSET1,		// command to ask client to change project blank length, interval, trigger 2 solenoid, server => client
	DM_CHANGE_PRJSET2,		// command to ask client to change project trigger 2 camera, light source template, server => client
	DM_NEWPRODUCTION,		// command to ask client to begin another new production, server => client

	DM_START_INSPECTION,	// command to ask client to start working, server => client
	DM_STOP_INSPECTION,		// command to ask other machines to stop machine, server => client
	DM_LOGINUSER,			// command to setup login user, server => client

	DM_CUR_LIGHTSOURCE,		// command to tell server the current connected light source info of client side, client => server
	DM_CLI_PROJECTINFO,		// command to tell server each client's project detail information, client => server
	DM_CHANGE_PAGE,			// command to tell client to force change to specified page, server <=> client
	DM_ENTER_SETGOLDEN,		// command to tell client to enter set golden sample mode, server <=> client
	DM_LEAVE_SETGOLDEN,		// command to tell client to leave set golden sample mode, server <=> client

	DM_SET_ANCHOR,			// command to tell server/client to remove all anchor points and add new anchor points, server <=> client
	DM_NEW_SEGMENT,			// command to tell server/client to add a new segment, server <=> client
	DM_DEL_SEGMENT,			// command to tell server/client to delete a new segment, server <=> client
	DM_REMOVE_SEGMENT,		// command to tell server/client to remove a segment, server <=> client	// added by eric at 20110802
	DM_MOD_SEGMENT,			// command to tell server/client to modify a new segment, server <=> client
	DM_MGE_SEGMENT,			// command to tell server/client to merge(delete/new) a new segment, server <=> client
	DM_DIFF_SEGMENT_KEY,	// command to tell server the changed segment's primary key of client, client => server
	DM_MOV_SEGMENT,			// command to tell server/client to move a segment, server <=> client
	DM_MOD_INSPREGION,		// command to tell server/client to modify current inspection region, server <=> client
	DM_ASK_CLEARPAGE2SETS,	// command to ask client to clear segments, anchors, inspection region server => client	// added by eric at 20140320
	DM_SET_CHECKMGCSTATE,	// command to ask client to checkMgcState // added by eric at 20140801

	DM_SET_DARKPNT,			// command to tell server/client to modify the serious check points of project setting, server <=> client
	//DM_SET_COLORDIFFCHECK,	// command to tell server/client to modify the color diff check of project setting, server <=> client
	//DM_SET_COLORDELTA,		// command to tell server/client to modify the color diff delta of project setting, server <=> client
	//DM_SET_PAPERSHRINK,		// command to tell server/client to modify the paper shrink points of project setting, server <=> client    no use
	//DM_SET_INKCHECK1,		// command to tell server/client to modify the horizontal ink check of project setting, server <=> client			no use
	//DM_SET_INKCHECK2,		// command to tell server/client to modify the vertical ink of project setting, server <=> client						no use
	//DM_SET_INKPERCENTAGE,	// command to tell server/client to modify the ink check percentage of project setting, server <=> client	no use
	//DM_CLI_UNLOCKWAIT,		// command to tell server the client has finished the action, the waiting lock can be unlock, client => server	no use
	//DM_CLI_INSPRGNBACK,		// command to tell server the re-generated inspection region from client, client => server								no use

	DM_START_SAMPLING,		// command to tell server/client to start sampling, server <=> client
	DM_STOP_SAMPLING,		// command to tell server/client to stop sampling, server <=> client
	DM_CLI_SETSAMPLEIMG,	// command to tell server the last captured sample image from client, client = server
	DM_CHECK_IDXSAMPLE,		// command to tell server/client to check specified sample image, server <=> client
	DM_DO_SETGOLDEN,		// command to tell client to do set golden image, server <=> client

	DM_CLI_INSPRESULT,		// command to tell server the inspection result from client side, client => server
	DM_ASK_INSPIMAGE,		// command to ask client send back the specified inspection image, server => client
	DM_CLI_INSPIMAGE,		// command to tell server the inspection image from client side, client => server
	//eric chao 20160222 remove DL
	//	DM_DEL_DLGROUP,			// command to tell client to delete the specified defect learning group, server => client
	DM_INSP_SAMPLES,		// command to ask client to do inspection on samples or send the sample inspection results back to server, server <=> client
	//	DM_ADD_DLGROUP_BYSAMPLE,	// command to tell client to add one defect learning group by using sample image, server => client
	//	DM_ADD_DLGROUP_BYINSPIMG,	// command to tell client to add one defect learning group by using inspected image, server => client
	//	DM_DIFF_DLGROUP_KEY,	// command to tell server the changed defect learning group's primary key of client, client => server
	//	DM_UPDATE_DLSIZE,		// command to tell server the defect learning size of client, client => server

	DM_ASK_INSPDETAIL,		// command to ask client to do the inspection on specified image(by index) and specified defect learning group, server => client
	DM_CLI_INSPDETAIL,		// command to tell server the inspection result detail from client side, including defect map and defect learning result, client => server

	DM_ASK_PRJ_PRODUCTION,	// command to ask/set all production of client, client <=> server
	DM_ASK_PRODUCTION_DETAIL,	// command to ask/set a production records of client, client <=> server	-- added by eric at 20120116
	DM_ASK_DIMG_DETAIL,		// command to ask/set one specified defect image record of one specified production, server <=> client

	DM_SET_ENCODERDIST,		// command to setup encoder distance, server => client
	DM_SET_SOLENOIDRT,		// command to setup solenoid distance, server => client
	//eric chao 20161220 remove
	//	DM_SET_PIXELDIST,		// command to setup pixel distance, server => client
	DM_SET_KEEPDAY,			// command to setup defected image keep day, server => client
	DM_SET_AUTOCLEAN,		// command to setup if auto clean, server => client
	DM_SET_HASULTRASONIC,	// command to setup enabling ultrasonic
	//DM_SET_HASTRIGGER2,		// command to setup enabling trigger2								no use
	//DM_SET_TG2DISTANCE,		// command to setup trigger 2 to solenoid distance					no use
	DM_SET_MAXSPEED,		// command to setup max speed, server => client	-- added by eric at 20120217

	DM_LEVEL1_CHANGE,		// command to ask client to do the level 1 change
	DM_LEVEL2_CHANGE,		// command to ask client to do the level 2 change
	DM_LEVEL3_CHANGE,		// command to ask client to do the level 3 change
	DM_GOLDENCHANGE,		// command to ask client to do Golden change -- added by eric at 20120919

	DM_ADD_USER,			// command to ask client to add a user
	DM_MOD_USER,			// command to ask client to modify a user's password
	DM_DEL_USER,			// command to ask client to delete a user

	DM_UPDATE_SEGMENT,		// command to update the inspection segment data from client to server, client => server, for import function
	DM_UPDATE_SEGMENT_EX,	// command to update the inspection segment data from server TO client //seanchen 20140728

	//	DM_SET_SPEED,			// command to tell server the current speed //eric chao 20171222 remove

	//DM_UNI_LIGHTLEVEL,		// command to ask client to change uni-lighting level -- added by eric at 20120706.eric chao 20131218 remove,no use now
	DM_ADD_LIGHTLEVEL,		// command to ask client to add lighting level
	DM_DES_LIGHTLEVEL,		// command to ask client to decrease lighting level
	//DM_ALL_LIGHTLEVEL,		// command to ask client to change all lighting level	-- added by eric at 20120712,eric chao 20131218 remove,no use now

	DM_CLIENT_ERRMSG,		// command to tell server the client error message

	DM_SET_SAVEALLIMG,		// command to ask client to save all rotated images for debug

	DM_CLI_SETWEAKENIMG,	// command to tell server the last captured weaken image from client, client = server
	DM_START_WEAKENLEARN,	// command to tell all client to start weaken learning
	DM_SET_WEAKENMAP,		// command to tell server the newest max/min maps
	DM_MOD_WEAKENMAP,		// command to ask client to modify its max/min maps	
	//DM_ADD_WEAKEN_BYINSPIMG,// command to ask client to do local weaken learn  //eric chao 20171117 no use remove
	DM_START_SINGLE_WEAKENLEARN,	// command to tell single client to start weaken learning
	DM_UNDOMOD_MAXMINMAP,
	DM_MOD_WEAKENMAP_NOSAVE,
	DM_RELOAD_MAXMINMAP,
	DM_SET_PRODUCTION,			// command to tell server the current production

	//eric chao 20150211 no use ------------------------
	DM_SET_IOCARD_DEVICEMODE,//DM_CLI_PRJINFO_START,
	DM_SET_DEFECT_OBJECT_INFO,//DM_CLI_PRJINFO_END, //eric chao 20150615
	//DM_INSP_SAMPLE_START,				no use
	//DM_INSP_SAMPLE_END,					no use
	//-----------------------

	DM_SET_IOVERSION,		// Command to tell server the I/O board version is acquired. -- Beagle 20110715
	// added by eric at 20110718
	DM_CLIENT_BLANKTOOCLOSE,	// command to display "blank too close" image
	DM_UNLOAD_PROJECT,		// Command to ask client to unload project and return to page 1 -- Beagle 20110719

	DM_ASK_FULLSAMPLEIMAGE,	// Command that request current client to send back sampling image. server => client -- Beagle 20110727
	DM_CLI_FULLSAMPLEIMAGE,	// Command for current client to send sampling image to server. client => server -- Beagle 20110727

	//DM_ASK_PRODUCTIONREPORT,	// Command to tell server/client to start production report, server <=> client	-- added by eric at 20110822		no use
	//DM_CLI_PRODUCTIONREPORT,	// Command to tell server to receive production report, server <=> client	-- added by eric at 20110822			no use

	DM_CHANGE_LANGUAGE,		// Command to tell clients to change UI language, server => client	-- added by eric at 20110825

	DM_ACK_START_INSPECTION,	// For clients to tell server ready to perform inspection. client => server -- Beagle 20110829

	DM_LOADPRJ_ERRMSG,	// command to tell server the client load project error message -- added by eric at 20110914

	DM_MOD_MAXMINMAP,		// command to ask client to modify its max/min maps	 --  added by eric at 20110929

	DM_SET_DEFECTFOLDER,	// command to ask client to modify its defect folder	 --  added by eric at 20110930

	DM_SET_BARCODEVALS,		// command to ask client to modify its barcode settings	 --  added by eric at 20111024
	DM_SET_BARCODEOCRIMGS,		// command to ask client to modify its barcode OCR imgs settings	 --  added by eric at 20111024
	DM_SET_BARCODEFILE,		// command to ask client to modify its barcode file	 --  added by eric at 20111024

	DM_NOTIFY_GRABBER_STOPPED,	// Client tell Master that grabber card is stopped. -- Beagle 20111123 added.

	DM_EXPORTPROJECT,			// command to ask client to export a project, server => client -- added by eric at 20111202
	DM_CLI_EXPORTPROJECT,		// command to tell server each client's project detail information, client => server -- added by eric at 20111202
	DM_IMPORTPROJECT,			// command to ask client to import a project, server => client -- added by eric at 20111202
	DM_CLI_EXPORT_ERRMSG,		// command to tell server the client export error message -- added by eric at 20111202
	DM_CLI_IMPORT_ERRMSG,		// command to tell server the client import error message -- added by eric at 20111202
	DM_UNDO_IMPORTPROJECT,		// command to tell client to undo import project -- added by eric at 20111202
	DM_ANCHORCHECK_MESSAGE,		// command to check anchor settings and show result -- added by eric at 20140619

	DM_ACK_DEFIMAGESIZEINFO,	// command to ask client to defect images size, server => client -- added by eric at 20120112
	DM_CLI_DEFIMAGESIZEINFO,	// command to tell server the client defect image size -- added by eric at 20120112
	DM_DELDEFECTIMGAGE,			// command to tell client to delete defect images -- added by eric at 20111206
	DM_CLI_DELDEFECTIMGAGE,		// command to tell server the client delete defect image size and the number that needs to delete -- added by eric at 20111206
	DM_DONE_DELDEFECTIMGAGE,	// command to tell client to finished to delete defect images -- added by eric at 20111206
	DM_CLI_DELETEPROJECT,		// Command to ask server to delete a project, client => server	-- added by eric at 20120528

	//DM_ADD_LIGHTINK_BYINSPIMG,	// command to ask client to do local Light & Ink learn	 -- added by eric at 20120106
	//DM_CLI_LIGHTINKMAP,			// command to tell server the client update learn maps -- added by eric at 20120106

	DM_CLI_COMPORT_ERR,			// Command to tell server the COM Port status. -- added by eric at 20120109
	DM_CLI_INSPECTION_ERR,		// Command to tell server the disable inspection. -- added by eric at 20120109
	DM_CLI_ACK_ENTER_SETGOLDEN,	// Acknowledge DM_ENTER_SETGOLDEN -- Beagle 20120106 added.
	DM_CLI_IOERROR_MSG,			// Command to tell server the IO Error. -- added by eric at 20120201
	DM_CLI_WARNING_MSG,			// Command to tell server to show warning message. -- added by amike at 20130704
	DM_CLIENT_SPEEDTOOFAST,		// command to display "speed too fast" error message	-- added by eric at 20120217
	DM_SET_SHRINKIMAGE,			// Command to setup client shrink rate of instant images. -- Beagle 20120220 added.
	DM_ASK_SETNEWBARCODE,		// command to ask client to backup barcode files	-- added by eric at 20120223
	DM_INFORM_INSPRESULT,		// command to tell client the inspection result of other clients, server => client -- Beagle 20120302 added.

	DM_ASK_BARCODELIST,			// command to ask client to send all barcode inspenction files list.	-- added by eric at 20120312
	DM_CLI_BARCODELIST,			// command to tell server the client selected barcode file list.	-- added by eric at 20120312
	DM_ASK_BARCORDFILE,			// command to ask client to send all barcode inspenction files.	-- added by eric at 20120312
	DM_CLI_BARCORDFILE,			// command to tell server the client selected barcode file.	-- added by eric at 20120312
	DM_CLI_BARCORDFILE_ERR,		// Command to tell server the barocde file Error. -- added by eric at 20120312
	DM_CLI_CAMERANUMBER,		// command to tell server the client set camera number.	-- added by eric at 20120506
	DM_SET_2DCODEINSPVALS,		// command to ask client to modify its 2D Code settings	 --  added by eric at 20120506
	DM_CLI_SETPRINTMODE,		// command to ask client to change printing test mode --  added by eric at 20120704
	DM_SET_CONNECTEDLSID,		// command to ask client to modify its connected light source ID setting	 --  added by eric at 20120810
	DM_SET_ALARMLIGHTVALS,		// command to ask client to modify alarm light setting	 --  added by eric at 20120810
	DM_CLI_WEBMODEFULLIMAGE,	// Command for current client to send sampling image to server. client => server -- added by eric at 20120817
	DM_SET_WEBMODESAMPLE,		// Command to tell client settings start point and offset size, server => client -- added by eric at 20120817
	DM_SET_WEBPRJVALS,			// command to ask client to modify its web mode settings	 --  added by eric at 20120822
	DM_SET_SOLENOID_MODE,		// Command to tell client to change I/O board solenoid mode. server => client	-- Beagle 20120905 added.
	DM_SET_PATTERNPOSVALS,		// command to ask client to modify its pattern three top-left position settings	 --  added by eric at 20121008
	DM_SET_PATTERNMAPMOD,		// command to ask client to modify its pattern map data	 --  added by eric at 20121008
	DM_SET_RESETSAMPLE,			// command to ask client to reset its golden max min images	 --  added by eric at 20121008
	DM_ASK_SEGMENTMAPIMG,		// command to ask client to send segment map & mask images	 --  added by eric at 20121019
	DM_NOTIFY_CMD,				// command to tell server the client complete  added by eric at 20121025
	DM_ASK_SOLENOIDTRIGGERTEST,	// command to ask client to do solenoid or page trigger test	-- added by eric at 20121030
	DM_FULLWEAKEN_BYINSPIMG,	// command to ask client to do full weaken learn	-- added by eric at 20121127
	DM_SET_LIGHTSOURCETEMPLATE,	// command to ask client to add a template light source -- added by eric at 20121130
	DM_ASK_REMOVEPRODUCTION,	// command to ask client to remove a production date -- added by eric at 20121205
	DM_SET_SEGMENTNUMBER,		// command to tell client number of segments to be sent from server -- added by eric at 20130109
	DM_SET_LAYOUTSETTING,		// command to tell client layout format from server -- added by eric at 20130109
	DM_SET_SEGMENTRELATION, 	// command to tell client project segments relation settings from server -- added by eric at 20130109
	DM_SET_IMAGEROTATION,		// command to tell client rotation start and stop position settings from server -- added by eric at 20130130
	DM_CLIENT_TOOFAST_NOECHO,	// command to tell server the client production too fast and io card sent blank decision no echo --- added by eric chao at 20130201
	DM_SET_CLIENT_EWF,			// command to tell client change ewf mode added by eric chao 20130204
	DM_NOTIFY_DETECT_CAM,		// command to tell server the client get the correct camera type added by eric chao 20130322
	DM_SET_IOPITCH,				// command to setup IO pulse, server => client
	DM_SET_VIEWORIGINALIMG,		// command to tell all clients to set show/hide original image check -- added by eric at 20130509
	DM_SET_MACHINETIME,			// command to tell all clients to be synchronized with server time -- added by eric at 20130522
	DM_START_FREERUN,			// command to tell server/client to start Free Run Images, server <=> client -- added by eric at 20130528
	DM_RESET_INSPECTION,		// command to tell client to reset current inspection -- eric chao 20130605
	DM_SHOW_DEFECTMAP,			// command to tell client to Show/Hide defect rect map -- eric chao 20130627
	DM_SET_SOLENOID,			// command to tell all clients to set solenoid check -- added by eric at 20130703
	DM_NORMAL_STREVNET,			// command for client and server (general purprse) (must delete memory after using) -- added by seanchen at 20130710
	DM_NORMAL_DYSTREVNET,		// command for client and server (general purprse) (must delete memory after using) -- added by seanchen at 20130710
	DM_SET_LIGHTSRCMAP,			// command to tell all clients to set Light Source Mapping -- added by eric at 20130724
	DM_SET_INPUTTRIGGERLINK,	// command to tell all clients to set total input trigger, each Input Trigger length, and solenoid link --- added by eric at 20130731
	DM_SET_ULTRASONICRESPONSETIME,	// command to tell all clients to set UltraSonic Response Time --- added by eric at 20130814
	DM_SET_EXPOSUREVALS,		// command to tell all clients to set Camera Exposure Value -- added by eric at 20130906
	DM_SET_FOVTIMES,			// command to tell all clients to set FOV Times -- added by eric at 20130906
	DM_SET_IOPINCAMERAMAP,		// command to tell all clients to set Frame Grabber Pin & Page Delay Pin with All Camera -- added by eric at 20130911
	DM_PRJ_INFO_QUERY,			// command to tell all clients to send all info to master --- eric chao 20130912
	DM_FILE_TRANSFER,			// command to trans file to client or to server   eric chao 20130914
	DM_ESTIMATEBLANKS,			// command to ask client to send Estimate Blanks -- added by eric at 20130917
	DM_SET_PAGEMODERUN,			// command to tell server to run PageMode -- eric chao 20131007
	DM_SET_BLANKBOXINSP,		// command to tell client to set Blank Box Inspection check -- added by eric at 20131115
	DM_NOTIFY_CONNECT_SUCCESS,	// command to tell server to client connect ok -- eric chao 20131115
	DM_HARDWARE_DETECTION,		//  command to tell client to set Hardware detection -- added by eric at 20131119
	DM_SET_SAMPLEEXTRAINFO,		// command to tell server all extra sample info --- eric chao 20131230
	DM_AUTO_UPDATE,				// command to tell client to Execute update process --- eric chao 20140102
	DM_AOI_RESTART,				// command to tell client to Restart AOI --- eric chao 20140113
	DM_SET_BELTLENGTHVALS,		// command to ask client to save Belt Length settings	 --  added by eric at 20140220
	DM_PREPARE_AUTO_UPDATE,		// command to tell client prepare Auto Update --- eric chao 20140308
	DM_SET_QCMODE,				// command to tell server QC Mode --- added by eric at 20140429
	DM_SET_WASTEZONEVALS,		// command to tell client to update Waste Paper Zone Settings --- added by eric at 20140506
	DM_SET_EXSOLENOIDVALS,		// command to tell client to update Extra Solenoid Output Pin settings --- added by eric at 20140515
	DM_SET_BEEPVALS,			// command to tell client to update Beep Alarm Output Pin settings --- added by eric at 20140515
	DM_SET_CONFIGIO,			// command to tell client to set Config IO settings --- added by eric at 20140515
	DM_SET_MISSTRIGGERVALS,		// command to tell client to set Miss Trigger settings --- added by eric at 20140520
	DM_SET_WEBMODE_START_POS,   // command to tell client to set WebMode Inspection Start Position ---- eric chao 20140728
	DM_WEB_INFO_EXPORT,			// command to tell master to finish get web export FILE --- eric chao 20140728
	DM_SET_DECODEINSPRELATION,	// command to tell client to set Decode & Inspection relation settings -- added by eric at 20140912
	DM_SET_ALIASDEFECTTYPE_KEY,	// command to tell master that defectType's Id in Alias table -- added by eric at 20140925
	DM_SET_BACKUPBARCODEFILE,	// command to tell client to backup its barcode file	 --  added by eric at 20140926
	DM_SET_TRANSCARDVERSION,	// Command to tell server the Trans board version is acquired. -- seanchen 20141001-01
	DM_RECHECK_IOCARD,			// Command to tell clint to run check IO Card flow -- seanchen 20141001-01
	DM_SET_CORPPROFILE,			// Command to tell client to set Corporation Profile --- added by eric at 20141006	
	DM_UNMASK_DEFECTIMG,		// Command to tell clint to unmask defect image or client tell master to unmask defect -- 未使用　以後可以使用
	DM_SET_ROTATIONCHECK,		// Command to tell client to change rotation check --- added by eric at 20141013
	DM_ASK_SEGBARCODEREPORT,	// Command to ask All clients to send segment barcode report files --- added by eric at 20141125
	DM_ASK_QRIMAGERECT,			// Command to ask client to send "calculator shift values" for QR Image --- added by eric at 20141209
	DM_SET_SPECIALINSPPROFILE,	// Command to tell client to set Special Insp Type value -- added by eric at 20150115
	DM_SEND_SEGTEMPLATE,		// Command to tell client to set Template value --- added by eric at 20150125
	DM_ASK_TEMPLATE_IMAGES,		// Command to ask client to send Template Images --- added by eric at 20150125
	DM_MOD_TEMPLATEMAXMINMAP,	// Command to tell client to save Template Golden, MaxMap, MinMap --- added by eric at 20150224
	DM_SET_DEFECTGROUPTYPE,		// Command to tell client to set Corp. set Defect Group Type values -- added by eric at 20150304
	//DM_SET_EDGESTATE,			// Command to tell client to set Check Edge Pixels -- added by eric at 20150330 // no use
	DM_SENDCUSTOMSEGMENTTAB,	// Command to tell client to save Custom Segment Tab Display settings -- added by eric at 20150415
	DM_QUERY_FREQMAP,			// Command to tell clinet to send Defect Freq Map --- eric chao 20150420
	DM_SET_FREQMAP,				// Command to tell master to Update Defect Freq Map --- eric chao 20150420
	DM_ASK_ANCHORPOINT,			// Command to ask client to send anchor point -- added by eric at 20150427
	DM_ASK_MODIFYCLRDERIVATE,	// Command to ask client to do auto modify color derivate segment -- added by eric at 20150430
	DM_RESP_BMP_FILE,			// command to tell server the client send BMP image -- seanchen 20150610-02
	DM_QUERY_DEFECT_OBJECT_ITEM,	// Command to ask client to send Defect Object Item --- eric chao 20150611
	DM_SET_REPRESENTIMAGE,		// Command to tell client to set Representative Image Settings -- added by eric at 20150617
	DM_WMCLOSE_NOTIFY,			// Command to tell client to do WM_CLOSE notify -- added by eric at 20150728
	DM_SET_GPIOLINK,			// command to tell client to set GPIO Link Settings ---- added by eric at 20150820
	DM_GET_QRENCODEPAGE,		// Command to ask client to check QR Encode Page ---- added by eric at 20151001
	DM_SET_REELMODE,			// command to tell client to set Reel Mode Settings ---- added by eric at 20151022
	DM_SET_WEBNOTRIGGERLEADER,	// command to tell client to find start for web mode ---- seanchen 20151015-01
	DM_ASK_EDGECHKMAP,			// command to ask client to send Edge Check Map ---- added by eric at 20151204
	DM_SET_INSPECTIONCHECK,		// command to tell client to set Inspection Check ---- added by eric at 20151222
	DM_SET_PDF_LAYER,			// command to tell client PDF Layer process		eric chao 20160118
	DM_SET_ENCRYPTMODE,			// command to tell client to set Encrypt Mode Settings ---- added by eric at 20160126
	DM_ASK_UPDATEIMPORTFILERELATION,	// command to tell client to update code file or relation ---- added by eric at 20160328
	DM_SET_REALTIMEIMAGE_RECT,	// command to tell client to set Real-time display area---- added by eric at 20160402
	DM_SET_REALTIMEIMAGE_STATUS,// command to tell client to set Real-time disable/enable ---- added by eric at 20160402
	DM_SEND_REALTIMEIMAGE,		// command to ask client to set Real-time image ---- added by eric at 20160402
	DM_SET_QRTYPE,				// command to ask client to set QR Type ---- added by eric at 20160413
	DM_SET_WEBUNITSETTINGS,		// command to ask client to set Web Unit settings ---- added by eric at 20160415
	DM_SET_T2ULTRASONICDIST,	// command to tell client to set Trigger to Solenoid distance Settings ---- added by eric at 20160420
	DM_SET_LABELINGVALS,		// command to tell client to update Labeling Output Pin settings --- added by eric at 20160504
	DM_SET_LABELINGOFFSETVALS,	// command to ask client to modify labeling offset settings	 --  added by eric at 20160505
	DM_SET_AUTOPRJ,				// command to tell client to set auto project for anchors and derivate segment Settings ---- added by eric at 20160515
	DM_SET_AUTOPRJSTEP,			// command to tell server to set auto project step information ---- added by eric at 20160515
	DM_SET_SAMPLEINDEX,			// command to tell client to set sample index ---- added by eric at 20160520
	DM_SET_HARDWAREDEFAULTVALS,	// command to ask client to set HW default settings ---- added by eric at 20160621
	DM_SET_NEWBATCHNOTIFYSET,	// command to ask client to set new batch notify check T2Notify dist settings ---- added by eric at 20160729
	DM_MSG_COMMAND_ITEM,		// command for MSG_ITEM_COMMAND
	DM_MOD_DECODE_EXTRA,		// command to modidy client Decode List eric chao 20160804
	DM_SET_INSP_RESULT_ITEM,	// command to tell server Insp Result //eric chao 20160812
	DM_SET_INSP_WEBEXPORTCACHE,	// command to tell server Cache Result Done	// added by eric at 20160914 
	DM_SEND_PDFIMAGE,			// Command to send a PDF file to client --- added by eric at 20160826
	DM_SET_FAESETTINGTVALS,		// command to ask client to set FAE settings ---- added by eric at 20161214
	DM_SET_INSPSKIPDIST,		// command to ask client to set new Inspection Skip Dist dist settings ---- added by eric at 20170510
	DM_MACHINE_REBOOT,			// command to ask client Machine Reboot //eric chao 20171011
	DM_AOICAPTURE_BATCH_ID,     // command to tell client batch-id
	DM_AOICAPTURE_CALCULATE_SIZE, // command to tell server capture size
	DM_SOLENOID_UI_CTRL,		//command to tell client Solenoid Ui Control
	DM_SET_LEDSETVALS,          //command to tell client to update LED settings --- added by eric at 20180207
	DM_SYNC_IMG,
} NDK_DataMessage;
