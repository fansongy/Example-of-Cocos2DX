package com.fansy.mySDK;

import org.cocos2dx.cpp.Cocos2dxActivity;

import android.os.Bundle;
import android.util.Log;
import android.widget.Toast;

import com.mappn.sdk.common.utils.BaseUtils;
import com.mappn.sdk.common.utils.ToastUtil;
import com.mappn.sdk.pay.GfanPay;
import com.mappn.sdk.uc.GfanUCCallback;
import com.mappn.sdk.uc.GfanUCenter;
import com.mappn.sdk.uc.User;

public class MainActivity extends Cocos2dxActivity{

	public native void OnLogin(int result,String userName);
	
	public static MainActivity s_Instance = null;

	public static Object rtnObject() {
	    return s_Instance;   
	}
	    
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
		Log.d("","Main create");
		
		GfanPay.getInstance(getApplicationContext()).init();
		s_Instance = this;
	}
	
	public void LogIn() {
		GfanUCenter.login(this, new GfanUCCallback() {

			private static final long serialVersionUID = 8082863654145655537L;

			@Override
			public void onSuccess(User user, int loginType) {
				// TODO 登录成功处理
				ToastUtil.showLong(getApplicationContext(),"登录成功 user：" + user.getUserName());
				OnLogin(0,user.getUserName());
			}

			@Override
			public void onError(int loginType) {
				Toast.makeText(getApplicationContext(), "登录失败",Toast.LENGTH_SHORT).show();
				OnLogin(0,"登陆失败");
			}
		});

	}
}
