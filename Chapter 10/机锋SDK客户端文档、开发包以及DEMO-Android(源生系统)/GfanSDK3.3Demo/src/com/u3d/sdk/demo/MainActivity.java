package com.u3d.sdk.demo;

import android.app.Activity;
import android.os.Bundle;
import android.text.TextUtils;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

import com.mappn.sdk.common.utils.BaseUtils;
import com.mappn.sdk.common.utils.ToastUtil;
import com.mappn.sdk.pay.GfanChargeCallback;
import com.mappn.sdk.pay.GfanConfirmOrderCallback;
import com.mappn.sdk.pay.GfanPay;
import com.mappn.sdk.pay.GfanPayCallback;
import com.mappn.sdk.pay.model.Order;
import com.mappn.sdk.statitistics.GfanPayAgent;
import com.mappn.sdk.uc.GfanUCCallback;
import com.mappn.sdk.uc.GfanUCenter;
import com.mappn.sdk.uc.User;
import com.mappn.sdk.uc.util.StringUtil;
import com.mappn.sdk.uc.util.UserUtil;

public class MainActivity extends Activity implements OnClickListener {

	private static final String TAG = MainActivity.class.getSimpleName();

	private TextView usernameView;
	private TextView uidView;
	private TextView runningNumView;
	private TextView orderidview;

	private EditText payValueText;
	private EditText orderIdText;
	private Button payButton;
	private Button orderButton;

	private Button chargeButton;
	private Button loginButton;
	private Button regButton;
	private Button logoutButton;
	private Button modfiyButton;
	private Button isonekeyBtn;
	
	private Button gameLogin;
	private Button gameRegist;
	private Button newRole;
	
	private Button btnCommunity;
	
//	private Button gameShare;
//	private Button inviteFriends;

	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		BaseUtils.D(TAG, "onCreate");
		setContentView(BaseUtils.get_R_Layout(getApplicationContext(), "gfan_main"));
		// Activity销毁时，事物未完成，恢复
		BaseUtils.D(TAG, "onekeyname"+UserUtil.getOnekeyName(this));
		initView();
		GfanPay.getInstance(getApplicationContext()).init();
		GfanPay.getInstance(getApplicationContext()).confirmPay(new GfanConfirmOrderCallback() {

			@Override
			public void onExist(Order order) {
				if (order != null) {
					BaseUtils.D("MainActivity", "订单号为：" + order.getOrderID());
				}
			}

			@Override
			public void onNotExist() {
				
			}
			
		});
		
		
	}

	@Override
	protected void onResume() {
		super.onResume();
		GfanPayAgent.onResume (this);
		BaseUtils.D(TAG, "getOnekeyName----:"+getOnekeyName("5ce2f47ae925", "864931010480383"));
		
		
	}

	
  	public static String getOnekeyName(String mac,String imei) {
			//MAC地址处理
			if (mac!=null) {
				mac = mac.replace(":", "");
			}else {
				return null;
			}
//			//IMEI处理
			if (imei!=null&&imei.length()>13) {
				imei = imei.substring(11);
			}else {
				imei = "9876";
			}
			String macHex64 = com.mappn.sdk.common.codec.binary.Base64.encodeBase64String(StringUtil.hexString2Bytes(mac));
			String imeiHex64 = com.mappn.sdk.common.codec.binary.Base64.encodeBase64String(StringUtil.hexString2Bytes(imei));
			String name = macHex64+imeiHex64;
			//去掉不能用的字符
			name = name.replace("\r", "");
			name = name.replace("\n", "");
			name = name.replace("=", "_");
			name = name.replace("+", "gf");
			name = name.replace("/", "fg");
			return name;
		}
	
	@Override
	protected void onPause() {
		super.onPause();
		GfanPayAgent.onPause(this);
	}
	
	private void initView() {
		usernameView = (TextView) findViewById(BaseUtils.get_R_id(getApplicationContext(), "tv_username"));
		uidView = (TextView) findViewById(BaseUtils.get_R_id(getApplicationContext(), "tv_uid"));
		runningNumView = (TextView) findViewById(BaseUtils.get_R_id(getApplicationContext(), "tv_runningNum"));
		orderidview = (TextView) findViewById(BaseUtils.get_R_id(getApplicationContext(), "tv_order_id"));

		payValueText = (EditText) findViewById(BaseUtils.get_R_id(getApplicationContext(), "et_pay_value"));
		orderIdText = (EditText) findViewById(BaseUtils.get_R_id(getApplicationContext(), "et_order_id"));
		payButton = (Button) findViewById(BaseUtils.get_R_id(getApplicationContext(), "btn_pay"));
		payButton.setOnClickListener(this);

		orderButton = (Button) findViewById(BaseUtils.get_R_id(getApplicationContext(), "btn_generate_orderid"));
		orderButton.setOnClickListener(this);

		chargeButton = (Button) findViewById(BaseUtils.get_R_id(getApplicationContext(), "btn_charge"));
		chargeButton.setOnClickListener(this);

		loginButton = (Button) findViewById(BaseUtils.get_R_id(getApplicationContext(), "btn_login"));
		loginButton.setOnClickListener(this);

		regButton = (Button) findViewById(BaseUtils.get_R_id(getApplicationContext(), "btn_register"));
		regButton.setOnClickListener(this);
		
		logoutButton = (Button) findViewById(BaseUtils.get_R_id(getApplicationContext(), "btn_logout"));
		logoutButton.setOnClickListener(this);

		gameLogin = (Button) findViewById(BaseUtils.get_R_id(getApplicationContext(), "btn_game_login"));
		gameLogin.setOnClickListener(this);

		gameRegist = (Button) findViewById(BaseUtils.get_R_id(getApplicationContext(), "btn_game_register"));
		gameRegist.setOnClickListener(this);

		newRole = (Button) findViewById(BaseUtils.get_R_id(getApplicationContext(), "btn_new_role"));
		newRole.setOnClickListener(this);
		
		modfiyButton = (Button) findViewById(BaseUtils.get_R_id(getApplicationContext(), "btn_modfiy"));
		modfiyButton.setOnClickListener(this);
		
		isonekeyBtn = (Button)findViewById(BaseUtils.get_R_id(getApplicationContext(), "btn_isonekey"));
		isonekeyBtn.setOnClickListener(this);
		
		btnCommunity = (Button)findViewById(BaseUtils.get_R_id(getApplicationContext(), "btn_community"));
		btnCommunity.setOnClickListener(this);
		
		
//		gameShare = (Button)findViewById(BaseUtils.get_R_id(getApplicationContext(), "btn_share"));
//		gameShare.setOnClickListener(this);
//		
//		inviteFriends = (Button)findViewById(BaseUtils.get_R_id(getApplicationContext(), "btn_invite"));
//		inviteFriends.setOnClickListener(this);
//		
//		findViewById(BaseUtils.get_R_id(getApplicationContext(), "btn_iftttdev")).setOnClickListener(this);
//		findViewById(BaseUtils.get_R_id(getApplicationContext(), "btn_iftttdevstop")).setOnClickListener(this);
	}

	private void generateOrderId() {
		// 这里只是模拟生成订单号
		String orderId = "order" + System.currentTimeMillis();
		orderIdText.setText(orderId);
	}

	@Override
	public void onClick(View v) {
		int id = v.getId();
		if (BaseUtils.get_R_id(getApplicationContext(), "btn_generate_orderid") == id) {
			generateOrderId();
		} else if (BaseUtils.get_R_id(getApplicationContext(), "btn_pay") == id) {
			startGfanPay();
		} else if (BaseUtils.get_R_id(getApplicationContext(), "btn_charge") == id) {
			startGfanCharge();
		} else if (BaseUtils.get_R_id(getApplicationContext(), "btn_login") == id) {
			startGfanLogin();
		} else if (BaseUtils.get_R_id(getApplicationContext(), "btn_register") == id) {
			startGfanRegister();
		} else if (BaseUtils.get_R_id(getApplicationContext(), "btn_logout") == id) {
			startGfanLogout();
		} else if (BaseUtils.get_R_id(getApplicationContext(), "btn_game_login") == id) {
			GfanPay.submitLogin(getApplicationContext());
		} else if (BaseUtils.get_R_id(getApplicationContext(), "btn_game_register") == id) {
			GfanPay.submitRegist(getApplicationContext());
		} else if (BaseUtils.get_R_id(getApplicationContext(), "btn_new_role") == id) {
			GfanPay.submitNewRole(getApplicationContext());
		} else if (BaseUtils.get_R_id(getApplicationContext(), "btn_modfiy") == id) {
			startModfiy();
//		}else if (BaseUtils.get_R_id(getApplicationContext(), "btn_share") == id) {
//			startShare();
//		}else if (BaseUtils.get_R_id(getApplicationContext(), "btn_invite") == id) {
//			inviteFriends();
//		}else if (BaseUtils.get_R_id(getApplicationContext(), "btn_iftttdev") == id) {
//			Iftttdev.start(this,Iftttdev.DEFAULT_ICON);
//		}else if (BaseUtils.get_R_id(getApplicationContext(), "btn_iftttdevstop") == id) {
//			Iftttdev.stop(this);
		}else if (BaseUtils.get_R_id(getApplicationContext(), "btn_isonekey") == id) {
			isOnekeyUser();
		}else if (BaseUtils.get_R_id(getApplicationContext(), "btn_community") == id) {
//			GfanCommunity.start(this);
		}
	}

	private void startGfanLogout() {
		GfanUCenter.logout(this);
	}
	
	private void startGfanLogin() {
		GfanUCenter.login(this, new GfanUCCallback() {

			private static final long serialVersionUID = 8082863654145655537L;

			@Override
			public void onSuccess(User user, int loginType) {
				// TODO 登录成功处理
				usernameView.setText(user.getUserName());
				uidView.setText(user.getUid() + "");
//				Toast.makeText(getApplicationContext(),"登录成功 user：" + user.getUserName(), Toast.LENGTH_SHORT)
//						.show();
				BaseUtils.D("token", user.getToken());
				ToastUtil.showLong(getApplicationContext(),"登录成功 user：" + user.getUserName());
			}

			@Override
			public void onError(int loginType) {
				Toast.makeText(getApplicationContext(), "登录失败",Toast.LENGTH_SHORT).show();
				usernameView.setText("");
				uidView.setText("");
			}
		});
	}
	
	private void startGfanRegister() {
		GfanUCenter.regist(this, new GfanUCCallback() {

			private static final long serialVersionUID = 6967267290737180773L;

			@Override
			public void onSuccess(User user, int loginType) {
				// 由登录页登录成功
				if (GfanUCenter.RETURN_TYPE_LOGIN == loginType) {
					// TODO 登录成功
					// 由登录页注册成功
				} else {
					// TODO 注册成功
				}
				usernameView.setText(user.getUserName());
				uidView.setText(user.getUid() + "");
				Toast.makeText(getApplicationContext(),
						"注册成功 user：" + user.getUserName(), Toast.LENGTH_SHORT)
						.show();
			}

			@Override
			public void onError(int loginType) {
				Toast.makeText(getApplicationContext(), "注册失败",
						Toast.LENGTH_SHORT).show();
			}
		});
	}

	private void startGfanPay() {
		String orderId = orderIdText.getText().toString().trim();
		String payStr = payValueText.getText().toString().trim();
		if (!checkPayMoney(payStr)) {
			return;
		}

		int payValue = Integer.valueOf(payStr);
		Order order = new Order("支付点名称1", "支付点描述2", payValue, orderId);
		GfanPay.getInstance(getApplicationContext()).pay(order,
				new GfanPayCallback() {

					@Override
					public void onSuccess(User user, Order order) {
						usernameView.setText(user.getUserName());
						uidView.setText(user.getUid() + "");
						runningNumView.setText(order.getNumber());
						orderidview.setText(order.getOrderID() + "");
						Toast.makeText(
								getApplicationContext(),
								"支付成功 user：" + user.getUserName() + "金额："
										+ order.getMoney(), Toast.LENGTH_SHORT)
								.show();
					}

					@Override
					public void onError(User user) {
						if (user != null) {
							usernameView.setText(user.getUserName());
							uidView.setText(user.getUid() + "");
							Toast.makeText(getApplicationContext(),
									"支付失败 user：" + user.getUserName(),
									Toast.LENGTH_SHORT).show();
						} else {
							Toast.makeText(getApplicationContext(), "用户未登录",
									Toast.LENGTH_SHORT).show();
						}
					}
				});

	}

	private void startGfanCharge() {

		GfanPay.getInstance(getApplicationContext()).charge(
				new GfanChargeCallback() {

					@Override
					public void onSuccess(User user) {
						if (user==null) {
							return;
						}
						usernameView.setText(user.getUserName());
						uidView.setText(user.getUid() + "");
						Toast.makeText(getApplicationContext(),
								"充值成功 user：" + user.getUserName(),
								Toast.LENGTH_SHORT).show();
					}

					@Override
					public void onError(User user) {
						if (user != null) {
							usernameView.setText(user.getUserName());
							uidView.setText(user.getUid() + "");
							Toast.makeText(getApplicationContext(),
									"充值失败 user：" + user.getUserName(),
									Toast.LENGTH_SHORT).show();
						} else {
							Toast.makeText(getApplicationContext(), "用户未登录",
									Toast.LENGTH_SHORT).show();
						}
					}
				});
	}

	/**
	 * 修改个人信息|只有一键登录的账号可以修改
	 * @description：
	 * @author xinge21
	 * @time 2012-11-14 下午2:56:22 void:
	 */
	private void startModfiy() {
		GfanUCenter.modfiy(this, new GfanUCCallback() {
			@Override
			public void onSuccess(User user, int returnType) {
				if (GfanUCenter.RETURN_TYPE_MODFIY == returnType) {
					//TODO  完善账号成功
					usernameView.setText(user.getUserName());
					uidView.setText(user.getUid() + "");
					ToastUtil.showLong(getApplicationContext(),"完善账号成功 user：" + user.getUserName());
				}
			}

			@Override
			public void onError(int returnType) {
				if (GfanUCenter.RETURN_TYPE_MODFIY == returnType) {
					//TODO  完善账号失败
				}
			}
		});
	}
	
	/**
	 * 分享应用到SNS
	 * @description：
	 * @author xinge21
	 * @time 2012-11-21 下午4:27:35 void:
	 */
//	private void startShare() {
//		GfanUCenter.shareGame(this, new GfanUCCallback() {
//			
//			private static final long serialVersionUID = 2657947945042188459L;
//
//			@Override
//			public void onSuccess(User user, int loginType) {
//				if (loginType == Api.ACTION_GET_APKURL) {
//					Toast.makeText(getApplicationContext(), "获得下载地址成功，开始调用分享",Toast.LENGTH_SHORT).show();
//				}
//			}
//			
//			@Override
//			public void onError(int loginType) {
//				if (loginType == Api.ACTION_GET_APKURL) {
//					Toast.makeText(getApplicationContext(), "获得下载地址失败，取消调用分享",Toast.LENGTH_SHORT).show();
//				}
//			}
//		});
//	}
	
	/**
	 * 邀请机锋论坛好友|发送站内信息方式
	 * @description：
	 * @author xinge21
	 * @time 2012-12-4 下午3:57:15 void:
	 */
//	private void inviteFriends() {
//		GfanUCenter.inviteFriend(this, new GfanUCCallback() {
//			private static final long serialVersionUID = 4816922689170296073L;
//
//			@Override
//			public void onSuccess(User user, int loginType) {
//				Toast.makeText(getApplicationContext(), "邀请好友成功",Toast.LENGTH_SHORT).show();
//			}
//			
//			@Override
//			public void onError(int type) {
//				switch (type) {
//				case Api.ACTION_GET_FRIENDS:
//					Toast.makeText(getApplicationContext(), "邀请好友失败：好友列表读取失败",Toast.LENGTH_SHORT).show();
//					break;
//				case Api.ACTION_GET_APKURL:
//					Toast.makeText(getApplicationContext(), "邀请好友失败：应用下载地址获取失败",Toast.LENGTH_SHORT).show();
//					break;
//				case Api.ACTION_SENT_MSG:
//					Toast.makeText(getApplicationContext(), "邀请好友失败：发送站内信失败",Toast.LENGTH_SHORT).show();
//					break;
//				default:
//					Toast.makeText(getApplicationContext(), "用户取消邀请",Toast.LENGTH_SHORT).show();
//				}
//				
//			}
//		});
//	}
	
	
	private boolean checkPayMoney(String text) {
		if (TextUtils.isEmpty(text)) {
			Toast.makeText(this, "亲,你肯定没有填支付金额啊!", Toast.LENGTH_SHORT).show();
			return false;
		} else {
			try {
				int payValue = Integer.valueOf(text);

				if (payValue == 0) {
					Toast.makeText(this, "亲,支付金额必须大于0吧!", Toast.LENGTH_SHORT)
							.show();
					return false;
				}
			} catch (NumberFormatException e) {
				Toast.makeText(this, "亲,支付金额填写有误!", Toast.LENGTH_SHORT);
				return false;
			}
		}
		return true;
	}

	/**
	 * 是否是一键登录用户
	 * @description：
	 * @author xinge21
	 * @time 2012-12-11 上午10:58:53 void:
	 */
	private void isOnekeyUser() {
		ToastUtil.showLong(getApplicationContext(), (GfanUCenter.isOneKey(this)?"是":"不是")+"游客账号");
	}

	@Override
	protected void onDestroy() {
		super.onDestroy();
		BaseUtils.D(TAG, "Actitivy destroy");
		GfanPay.getInstance(getApplicationContext()).onDestroy();
	}
}