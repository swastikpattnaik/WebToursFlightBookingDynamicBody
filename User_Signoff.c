User_Signoff()
{
	
	web_reg_find("Text=Web Tours Navigation Bar", 
	             "SaveCount=WebTours_Count",
				 LAST);
	
	lr_start_transaction("SC02_BookingFlight_T07_SignOff");

	web_url("SignOff Button", 
		"URL=http://{pServerName}:{pPortNo}/cgi-bin/welcome.pl?signOff=1", 
		"TargetFrame=body", 
		"Resource=0", 
		"RecContentType=text/html", 
		"Referer=http://{pServerName}:{pPortNo}/cgi-bin/nav.pl?page=menu&in=home", 
		"Snapshot=t10.inf", 
		"Mode=HTML", 
		LAST);

	if(atoi(lr_eval_string("{WebTours_Count}")) > 0)
	{
		lr_end_transaction("SC02_BookingFlight_T07_SignOff", LR_PASS);
	}
	else
	{
		lr_end_transaction("SC02_BookingFlight_T07_SignOff", LR_FAIL);	
		lr_output_message("VUser ID %d encountered an error in Iteration No. %d.", atoi(lr_eval_string("{pVUserID}")), atoi(lr_eval_string("{pIterationNumber}")));
	}
	
	return 0;
}
