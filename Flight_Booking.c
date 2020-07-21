Flight_Booking()
{
	
/*
	***Departure City***
	<option selected="selected" value="Denver">Denver</option>
	<option value="Frankfurt">Frankfurt</option>
	<option value="London">London</option>
	<option value="Los Angeles">Los Angeles</option>
	<option value="Paris">Paris</option>
	<option value="Portland">Portland</option>
	<option value="San Francisco">San Francisco</option>
	<option value="Seattle">Seattle</option>
	<option value="Sydney">Sydney</option>
	<option value="Zurich">Zurich</option>
*/	

	web_reg_save_param_regexp(
		"ParamName=cDepartureCity",
		"RegExp=<option([a-z|=|\"| ]*) value=\"([A-Z|a-z| ]+)\"",
		"Group=2",
		"Ordinal=ALL",
		SEARCH_FILTERS,
		LAST);

/*
	***Arrival City***
	<option selected="selected" value="Denver">Denver</option>
	<option value="Frankfurt">Frankfurt</option>
	<option value="London">London</option>
	<option value="Los Angeles">Los Angeles</option>
	<option value="Paris">Paris</option>
	<option value="Portland">Portland</option>
	<option value="San Francisco">San Francisco</option>
	<option value="Seattle">Seattle</option>
	<option value="Sydney">Sydney</option>
	<option value="Zurich">Zurich</option>
*/	

	web_reg_save_param_regexp(
		"ParamName=cArrivalCity",
		"RegExp=<option([a-z|=|\"| ]*) value=\"([A-Z|a-z| ]+)\"",
		"Group=2",
		"Ordinal=ALL",
		SEARCH_FILTERS,
		LAST);
	
//	<input type="radio" name="seatPref" value="Window" />

	web_reg_save_param_ex(
		"ParamName=cSeatPreference",
		"LB=<input type=\"radio\" name=\"seatPref\" value=\"",
		"RB=\"",
		"Ordinal=ALL",
		SEARCH_FILTERS,
		LAST);
	
//	<input type="radio" name="seatType" value="First" />	

	web_reg_save_param_ex(
		"ParamName=cSeatType",
		"LB=<input type=\"radio\" name=\"seatType\" value=\"",
		"RB=\"",
		"Ordinal=ALL",
		SEARCH_FILTERS,
		LAST);
	
//	<input type="hidden" name=".cgifields" value="roundtrip"  />

	web_reg_save_param_ex(
		"ParamName=cCGIField1",
		"LB=<input type=\"hidden\" name=\".cgifields\" value=\"",
		"RB=\"",
		"Ordinal=1",
		SEARCH_FILTERS,
		LAST);

//	<input type="hidden" name=".cgifields" value="seatType"  />

	web_reg_save_param_ex(
		"ParamName=cCGIField2",
		"LB=<input type=\"hidden\" name=\".cgifields\" value=\"",
		"RB=\"",
		"Ordinal=2",
		SEARCH_FILTERS,
		LAST);
		
//	<input type="hidden" name=".cgifields" value="seatPref"  />
	
	web_reg_save_param_ex(
		"ParamName=cCGIField3",
		"LB=<input type=\"hidden\" name=\".cgifields\" value=\"",
		"RB=\"",
		"Ordinal=3",
		SEARCH_FILTERS,
		LAST);

//	<input type="hidden" name="advanceDiscount" value="0"  />

	web_reg_save_param_ex(
		"ParamName=cAdvDiscount",
		"LB=<input type=\"hidden\" name=\"advanceDiscount\" value=\"",
		"RB=\"",
		"Ordinal=1",
		SEARCH_FILTERS,
		LAST);
		
	web_reg_find("Text=Find Flight",
	             "SaveCount=FindFlight_Count",
				 LAST);
				 
	lr_start_transaction("SC02_BookingFlight_T03_ClickFlights");

	web_url("Search Flights Button", 
		"URL=http://{pServerName}:{pPortNo}/cgi-bin/welcome.pl?page=search", 
		"TargetFrame=body", 
		"Resource=0", 
		"RecContentType=text/html", 
		"Referer=http://{pServerName}:{pPortNo}/cgi-bin/nav.pl?page=menu&in=home", 
		"Snapshot=t4.inf", 
		"Mode=HTML", 
		LAST);

	if(atoi(lr_eval_string("{FindFlight_Count}")) > 0)
	{
		lr_end_transaction("SC02_BookingFlight_T03_ClickFlights", LR_PASS);
	}
	else
	{
		lr_end_transaction("SC02_BookingFlight_T03_ClickFlights", LR_FAIL);	
		lr_output_message("VUser ID %d encountered an error in Iteration No. %d.", atoi(lr_eval_string("{pVUserID}")), atoi(lr_eval_string("{pIterationNumber}")));
	}

	web_convert_param("tomDateFormat",  "SourceString={pDeptDate}", "SourceEncoding=PLAIN", "TargetEncoding=URL", LAST);

	web_convert_param("nextDateFormat",  "SourceString={pReturnDate}", "SourceEncoding=PLAIN", "TargetEncoding=URL", LAST);
	
	lr_output_message("The Departure Cities are:");
	
	while(i<=atoi(lr_eval_string("{cDepartureCity_count}")))
	{
		sprintf(deptCityVal, "{cDepartureCity_%d}", i);
		lr_output_message("%s", lr_eval_string(deptCityVal));
		i++;
	}

	do
	{
		deptCity = lr_paramarr_random("cDepartureCity");		
		arvlCity = lr_paramarr_random("cArrivalCity");
	}
	while(strcmp(arvlCity,deptCity) == 0);
	
	lr_save_string(deptCity, "pDepartureCity");
		
	lr_save_string(arvlCity, "pArrivalCity");
	
	seatPref = lr_paramarr_random("cSeatPreference");
	
	lr_save_string(seatPref, "pSeatPreference");
	
	seatType = lr_paramarr_random("cSeatType");
	
	lr_save_string(seatType, "pSeatType");
	
	lr_param_sprintf("pReqBody", "");		
		
	lr_param_sprintf("pReqBody", "%sadvanceDiscount=%d&depart=%s&arrive=%s&numPassengers=%d&seatPref=%s&seatType=%s&findFlights.x=49&findFlights.y=5&.cgifields=%s&.cgifields=%s&.cgifields=%s&departDate=%s&returnDate=%s",
	                 lr_eval_string("{pReqBody}"), atoi(lr_eval_string("{cAdvDiscount}")), lr_eval_string("{pDepartureCity}"), lr_eval_string("{pArrivalCity}"), atoi(lr_eval_string("{pPassengerNo}")), seatPref, seatType,
	                 lr_eval_string("{cCGIField1}"), lr_eval_string("{cCGIField2}"), lr_eval_string("{cCGIField3}"), lr_eval_string("{tomDateFormat}"), lr_eval_string("{nextDateFormat}"));

	if(atoi(lr_eval_string("{pRandRoundTrip}"))%2 == 0)
	{
		lr_param_sprintf("pReqBody", "%s&%s", lr_eval_string("{pReqBody}"), "roundtrip=on");
	}

//	lr_output_message(lr_eval_string("{pReqBody}"));

	lr_think_time(10);
	
	lr_output_message("Departure Date: %s", lr_eval_string("{pDeptDate}"));
	
	lr_output_message("Arrival Date: %s", lr_eval_string("{pReturnDate}"));
	
//	<input type="radio" name="outboundFlight" value="362;107;05/27/2020">

	web_reg_save_param_ex(
		"ParamName=cOutboundFlight",
		"LB=<input type=\"radio\" name=\"outboundFlight\" value=\"",
		"RB=\"",
		"Ordinal=ALL",
		SEARCH_FILTERS,
		LAST);
	
//	<input type="radio" name="returnFlight" value="633;92;05/28/2020">

	if(atoi(lr_eval_string("{pRandRoundTrip}"))%2 == 0)
	{
		web_reg_save_param_ex(
			"ParamName=cReturnFlight",
			"LB=<input type=\"radio\" name=\"returnFlight\" value=\"",
			"RB=\"",
			"Ordinal=ALL",
			SEARCH_FILTERS,
			LAST);
	}
	
	web_reg_find("Text=Flight departing from <B>{pDepartureCity}</B> to <B>{pArrivalCity}</B> on <B>{pDeptDate}</B>",
				 "SaveCount=FlightMessage_Count",	             
				 LAST);
	
	lr_start_transaction("SC02_BookingFlight_T04_FindFlights");
	
	web_custom_request("reservations.pl", 
		"URL=http://{pServerName}:{pPortNo}/cgi-bin/reservations.pl", 
		"Method=POST", 
		"Resource=0", 
		"RecContentType=text/html", 
		"Referer=http://{pServerName}:{pPortNo}/cgi-bin/reservations.pl?page=welcome", 
		"Snapshot=t5.inf", 
		"Mode=HTTP", 
		"Body={pReqBody}",
		LAST);
	
/*	
	web_submit_data("reservations.pl", 
		"Action=http://{pServerName}:{pPortNo}/cgi-bin/reservations.pl", 
		"Method=POST", 
		"TargetFrame=", 
		"RecContentType=text/html", 
		"Referer=http://{pServerName}:{pPortNo}/cgi-bin/reservations.pl?page=welcome", 
		"Snapshot=t5.inf", 
		"Mode=HTML", 
		ITEMDATA, 
		"Name=advanceDiscount", "Value={cAdvDiscount}", ENDITEM, 
		"Name=depart", "Value={pDepartureCity}", ENDITEM, 
		"Name=departDate", "Value={pDeptDate}", ENDITEM, 
		"Name=arrive", "Value={pArrivalCity}", ENDITEM, 
		"Name=returnDate", "Value={pReturnDate}", ENDITEM, 
		"Name=numPassengers", "Value=2", ENDITEM, 
		"Name=roundtrip", "Value=on", ENDITEM, 
		"Name=seatPref", "Value={pSeatPreference}", ENDITEM, 
		"Name=seatType", "Value={pSeatType}", ENDITEM, 
		"Name=findFlights.x", "Value=38", ENDITEM, 
		"Name=findFlights.y", "Value=9", ENDITEM, 
		"Name=.cgifields", "Value={cCGIField1}", ENDITEM, 
		"Name=.cgifields", "Value={cCGIField2}", ENDITEM, 
		"Name=.cgifields", "Value={cCGIField3}", ENDITEM, 
		LAST);
*/

	if(atoi(lr_eval_string("{FlightMessage_Count}")) > 0)
	{
		lr_end_transaction("SC02_BookingFlight_T04_FindFlights", LR_PASS);
	}
	else
	{
		lr_end_transaction("SC02_BookingFlight_T04_FindFlights", LR_FAIL);	
		lr_output_message("VUser ID %d encountered an error in Iteration No. %d.", atoi(lr_eval_string("{pVUserID}")), atoi(lr_eval_string("{pIterationNumber}")));
	}

	outboundFlight = lr_paramarr_random("cOutboundFlight");
	
	lr_save_string(outboundFlight, "pOutboundFlight");
	
	web_convert_param("pDepartureFlight",  "SourceString={pOutboundFlight}", "SourceEncoding=PLAIN", "TargetEncoding=URL", LAST);
	
	lr_param_sprintf("pReqBody1", "");		
		
	lr_param_sprintf("pReqBody1", "%snumPassengers=%d&advanceDiscount=%d&seatType=%s&seatPref=%s&reserveFlights.x=49&reserveFlights.y=5&outboundFlight=%s",
	                 lr_eval_string("{pReqBody1}"), atoi(lr_eval_string("{pPassengerNo}")), atoi(lr_eval_string("{cAdvDiscount}")), 
	                 seatType, seatPref, lr_eval_string("{pDepartureFlight}"));
	
	if(atoi(lr_eval_string("{pRandRoundTrip}"))%2 == 0)
	{
		returnFlight = lr_paramarr_random("cReturnFlight");
		
		lr_save_string(returnFlight, "pReturnFlight");

		web_convert_param("pArrivalFlight",  "SourceString={pReturnFlight}", "SourceEncoding=PLAIN", "TargetEncoding=URL", LAST);
		
		lr_param_sprintf("pReqBody1", "%s&returnFlight=%s", lr_eval_string("{pReqBody1}"), lr_eval_string("{pArrivalFlight}"));
	}

//	lr_output_message(lr_eval_string("{pReqBody1}"));
		
	lr_think_time(10);

//	<input type="checkbox" name="saveCC" value="on" checked="checked" />
	
	web_reg_save_param_ex(
		"ParamName=cSaveCC",
		"LB=<input type=\"checkbox\" name=\"saveCC\" value=\"",
		"RB=\"",
		"Ordinal=1",
		SEARCH_FILTERS,
		LAST);
	
//	<input type="hidden" name="oldCCOption" value="on"  />

	web_reg_save_param_ex(
		"ParamName=cOldCCOption",
		"LB=<input type=\"hidden\" name=\"oldCCOption\" value=\"",
		"RB=\"",
		"Ordinal=1",
		SEARCH_FILTERS,
		LAST);
	
//	<input type="hidden" name="JSFormSubmit" value="off"  />

	web_reg_save_param_ex(
		"ParamName=cJSFormSubmit",
		"LB=<input type=\"hidden\" name=\"JSFormSubmit\" value=\"",
		"RB=\"",
		"Ordinal=1",
		SEARCH_FILTERS,
		LAST);
	
//	<input type="hidden" name=".cgifields" value="saveCC"  />
	
	web_reg_save_param_ex(
		"ParamName=cCGIFields4",
		"LB=<input type=\"hidden\" name=\".cgifields\" value=\"",
		"RB=\"",
		"Ordinal=1",
		SEARCH_FILTERS,
		LAST);
	
	lr_start_transaction("SC02_BookingFlight_T05_SelectFlight");

	web_reg_find("Text=Save this Credit Card Information", 
				 "SaveCount=SaveCCardInfo_Count",
	             LAST);

	web_custom_request("reservations.pl_2", 
		"URL=http://{pServerName}:{pPortNo}/cgi-bin/reservations.pl", 
		"Method=POST", 
		"Resource=0", 
		"RecContentType=text/html", 
		"Referer=http://{pServerName}:{pPortNo}/cgi-bin/reservations.pl", 
		"Snapshot=t6.inf", 
		"Mode=HTTP", 
		"Body={pReqBody1}",
		LAST);

/*	
	web_submit_data("reservations.pl_2", 
		"Action=http://{pServerName}:{pPortNo}/cgi-bin/reservations.pl", 
		"Method=POST", 
		"TargetFrame=", 
		"RecContentType=text/html", 
		"Referer=http://{pServerName}:{pPortNo}/cgi-bin/reservations.pl", 
		"Snapshot=t6.inf", 
		"Mode=HTML", 
		ITEMDATA, 
		"Name=outboundFlight", "Value={pOutboundFlight}", ENDITEM, 
		"Name=returnFlight", "Value={pReturnFlight}", ENDITEM, 
		"Name=numPassengers", "Value=2", ENDITEM, 
		"Name=advanceDiscount", "Value={cAdvDiscount}", ENDITEM, 
		"Name=seatType", "Value={pSeatType}", ENDITEM, 
		"Name=seatPref", "Value={pSeatPreference}", ENDITEM, 
		"Name=reserveFlights.x", "Value=35", ENDITEM, 
		"Name=reserveFlights.y", "Value=8", ENDITEM, 
		LAST);
*/

	if(atoi(lr_eval_string("{SaveCCardInfo_Count}")) > 0)
	{
		lr_end_transaction("SC02_BookingFlight_T05_SelectFlight", LR_PASS);
	}
	else
	{
		lr_end_transaction("SC02_BookingFlight_T05_SelectFlight", LR_FAIL);	
		lr_output_message("VUser ID %d encountered an error in Iteration No. %d.", atoi(lr_eval_string("{pVUserID}")), atoi(lr_eval_string("{pIterationNumber}")));
	}

	lr_think_time(10);

	web_convert_param("pCreditCardNumber",  "SourceString={pRandNum} {pRandNum} {pRandNum} {pRandNum}", "SourceEncoding=PLAIN", "TargetEncoding=URL", LAST);
			
	web_convert_param("pExpDate",  "SourceString={pExpiryDate}", "SourceEncoding=PLAIN", "TargetEncoding=URL", LAST);
					
	lr_param_sprintf("pReqBody2", "");		
		
	lr_param_sprintf("pReqBody2", "%sfirstName=%s&lastName=%s&address1=%s&address2=%s&creditCard=%s&expDate=%s&numPassengers=%d&seatType=%s&seatPref=%s&outboundFlight=%s&advanceDiscount=%d&buyFlights.x=29&buyFlights.y=7&saveCC=%s&oldCCOption=%s&JSFormSubmit=%s&.cgifields=%s",
	                 lr_eval_string("{pReqBody2}"), lr_eval_string("{pFirstName}"), lr_eval_string("{pLastName}"), lr_eval_string("{pAddress1}"), 
	                 lr_eval_string("{pAddress2}"), lr_eval_string("{pCreditCardNumber}"), lr_eval_string("{pExpDate}"), atoi(lr_eval_string("{pPassengerNo}")), seatType, seatPref, 
	                 lr_eval_string("{pDepartureFlight}"), atoi(lr_eval_string("{cAdvDiscount}")),lr_eval_string("{cSaveCC}"), lr_eval_string("{cOldCCOption}"), 
	                 lr_eval_string("{cJSFormSubmit}"), lr_eval_string("{cCGIFields4}"));
	
	for(i=1; i<=atoi(lr_eval_string("{pPassengerNo}")); i++)
	{
		web_convert_param("pPassengerName",  "SourceString={pFirstName} {pLastName}", "SourceEncoding=PLAIN", "TargetEncoding=URL", LAST);
			
		lr_param_sprintf("pReqBody2", "%s&pass%d=%s", lr_eval_string("{pReqBody2}"), i, lr_eval_string("{pPassengerName}"));	
	}
	  
	if(atoi(lr_eval_string("{pRandRoundTrip}"))%2 == 0)
	{
		lr_param_sprintf("pReqBody2", "%s&returnFlight=%s", lr_eval_string("{pReqBody2}"), lr_eval_string("{pArrivalFlight}"));		
	}
	else
	{
		lr_param_sprintf("pReqBody2", "%s&returnFlight=", lr_eval_string("{pReqBody2}"));	
	}
                
//	lr_output_message(lr_eval_string("{pReqBody2}"));   
	
	lr_start_transaction("SC02_BookingFlight_T06_SetPaymentDetails");

	web_reg_find("Text=Flight reserved",
				 "SaveCount=FlightReserved_Count",
	             LAST);

	web_custom_request("reservations.pl_3", 
		"URL=http://{pServerName}:{pPortNo}/cgi-bin/reservations.pl", 
		"Method=POST", 
		"Resource=0", 
		"RecContentType=text/html", 
		"Referer=http://{pServerName}:{pPortNo}/cgi-bin/reservations.pl", 
		"Snapshot=t7.inf", 
		"Mode=HTTP", 
		"Body={pReqBody2}",
		LAST);
	
/*	
	web_submit_data("reservations.pl_3", 
		"Action=http://{pServerName}:{pPortNo}/cgi-bin/reservations.pl", 
		"Method=POST", 
		"TargetFrame=", 
		"RecContentType=text/html", 
		"Referer=http://{pServerName}:{pPortNo}/cgi-bin/reservations.pl", 
		"Snapshot=t7.inf", 
		"Mode=HTML", 
		ITEMDATA, 
		"Name=firstName", "Value={pFirstName}", ENDITEM, 
		"Name=lastName", "Value={pLastName}", ENDITEM, 
		"Name=address1", "Value={pAddress1}", ENDITEM, 
		"Name=address2", "Value={pAddress2}", ENDITEM, 
		"Name=pass1", "Value={pFirstName} {pLastName}", ENDITEM, 
		"Name=pass2", "Value={pFirstName} {pLastName}", ENDITEM, 
		"Name=creditCard", "Value={pRandNum} {pRandNum} {pRandNum} {pRandNum}", ENDITEM, 
		"Name=expDate", "Value={pExpiryDate}", ENDITEM, 
		"Name=saveCC", "Value={cSaveCC}", ENDITEM, 
		"Name=oldCCOption", "Value={cOldCCOption}", ENDITEM, 
		"Name=numPassengers", "Value=2", ENDITEM, 
		"Name=seatType", "Value={pSeatType}", ENDITEM, 
		"Name=seatPref", "Value={pSeatPreference}", ENDITEM, 
		"Name=outboundFlight", "Value={pOutboundFlight}", ENDITEM, 
		"Name=advanceDiscount", "Value={cAdvDiscount}", ENDITEM, 
		"Name=returnFlight", "Value={pReturnFlight}", ENDITEM, 
		"Name=JSFormSubmit", "Value={cJSFormSubmit}", ENDITEM, 
		"Name=buyFlights.x", "Value=40", ENDITEM, 
		"Name=buyFlights.y", "Value=5", ENDITEM, 
		"Name=.cgifields", "Value={cCGIFields4}", ENDITEM, 
		LAST);
*/

	if(atoi(lr_eval_string("{FlightReserved_Count}")) > 0)
	{
		lr_end_transaction("SC02_BookingFlight_T06_SetPaymentDetails", LR_PASS);
	}
	else
	{
		lr_end_transaction("SC02_BookingFlight_T06_SetPaymentDetails", LR_FAIL);	
		lr_output_message("VUser ID %d encountered an error in Iteration No. %d.", atoi(lr_eval_string("{pVUserID}")), atoi(lr_eval_string("{pIterationNumber}")));
	}

	lr_think_time(10);

	i = 1;
	
	lr_param_sprintf("pReqBody", "");	
		
	lr_param_sprintf("pReqBody1", "");

	lr_param_sprintf("pReqBody2", "");		
	
	return 0;
}