package com.fansy.shareSDK;

import org.cocos2dx.cpp.Cocos2dxActivity;

import android.os.Bundle;
import android.util.Log;

import com.tencent.mm.sdk.openapi.IWXAPI;
import com.tencent.mm.sdk.openapi.SendMessageToWX;
import com.tencent.mm.sdk.openapi.WXAPIFactory;
import com.tencent.mm.sdk.openapi.WXMediaMessage;
import com.tencent.mm.sdk.openapi.WXTextObject;

public class MainActivity extends Cocos2dxActivity{

	private static final String APP_ID = "yourself app id";
	private IWXAPI api;
	
	public static MainActivity s_Instance = null;

	public static Object rtnObject() {
	    return s_Instance;   
	}

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
		Log.d("sss","Main create");
		
		api = WXAPIFactory.createWXAPI(this, APP_ID,true);
		api.registerApp(APP_ID);
		s_Instance = this;
	}	
	
	public void sendToWeiXin(String str)
	{
		WXTextObject textObj = new WXTextObject();
		textObj.text = str;
		
		WXMediaMessage msg = new WXMediaMessage();
		msg.mediaObject = textObj;
		msg.description = str;
		
		SendMessageToWX.Req req = new SendMessageToWX.Req();
		req.transaction = String.valueOf(System.currentTimeMillis());
		req.message = msg;
		
		api.sendReq(req);
	}
}

