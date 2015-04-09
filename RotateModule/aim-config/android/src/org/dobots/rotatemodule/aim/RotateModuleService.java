package org.dobots.rotatemodule.aim;

import java.util.ArrayList;
import java.util.List;

import android.app.Service;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.ServiceConnection;
import android.os.AsyncTask;
import android.os.Bundle;
import android.os.Handler;
import android.os.IBinder;
import android.os.Message;
import android.os.Messenger;
import android.os.RemoteException;
import android.util.Log;


public class RotateModuleService extends Service {
	private static final String TAG = "RotateModuleService";
	private static final String MODULE_NAME = "RotateModule";
	
	Messenger mToMsgService = null;
	final Messenger mFromMsgService = new Messenger(new IncomingMsgHandler());
	boolean mMsgServiceIsBound;


	// TODO: Has to be generated!
//	Messenger mPortOutMessenger = null;
//	private List<Float> mPortOutBuffer = new ArrayList<Float>(0);
	
//	Messenger mPortInMessgener = new Messenger(new PortInMsgHandler());
	
	AIMRun mAIMRun;

	public void onCreate() {
		bindToMsgService();
		
		Integer id = 0; // TODO: adjustable id, multiple modules
		mAIMRun = new AIMRun();
		mAIMRun.execute(id);
	}
	
	public void onDestroy() {
		super.onDestroy();
		mAIMRun.cancel(true);
		unbindFromMsgService();
		Log.d(TAG, "onDestroy");
	}
	
	@Override
	public IBinder onBind(final Intent intent) {
		Log.i(TAG,"onBind: " + intent.toString());
		return null; // No binding provided
//		return mFromMsgService.getBinder();
	}
	
	// Called when all clients have disconnected from a particular interface of this service.
	@Override
	public boolean onUnbind(final Intent intent) {
		return super.onUnbind(intent);
	}
	
	// Deprecated since API level 5 (android 2.0)
	@Override
	public void onStart(Intent intent, int startId) {
//		handleStartCommand(intent);
	}
	
	// Called each time a client uses startService()
	@Override
	public int onStartCommand(Intent intent, int flags, int startId) {
//	    handleStartCommand(intent);
	    // We want this service to continue running until it is explicitly stopped, so return sticky.
	    return START_STICKY;
	}
	
	
	
	// Copied from MsgService, should be an include?
	public static final int MSG_REGISTER = 1;
	public static final int MSG_UNREGISTER = 2;
	public static final int MSG_SET_MESSENGER = 3;
	public static final int MSG_START = 4;
	public static final int MSG_STOP = 5;
	public static final int MSG_SEND = 6;
	public static final int MSG_XMPP_LOGIN = 7;
	public static final int MSG_ADD_PORT = 8;
	public static final int MSG_REM_PORT = 9;
	public static final int MSG_XMPP_LOGGED_IN = 10;
	public static final int MSG_XMPP_DISCONNECTED = 11;
	public static final int MSG_PORT_DATA = 12;
	public static final int MSG_USER_LOGIN = 13;
	public static final int MSG_GET_MESSENGER = 14;
		
	public static final int DATATYPE_FLOAT = 1;
	public static final int DATATYPE_FLOAT_ARRAY = 2;
	public static final int DATATYPE_STRING = 3;
	public static final int DATATYPE_IMAGE = 4;
	public static final int DATATYPE_BINARY = 5;
	
	

	
	
	private ServiceConnection mMsgServiceConnection = new ServiceConnection() {
		public void onServiceConnected(ComponentName className, IBinder service) {
			// This is called when the connection with the service has been established, giving us the service object
			// we can use to interact with the service.  We are communicating with our service through an IDL 
			// interface, so get a client-side representation of that from the raw service object.
			mToMsgService = new Messenger(service);
			//Log.i(TAG, "Connected to Dodedodo.");

			Message msg = Message.obtain(null, MSG_REGISTER);
			Bundle bundle = new Bundle();
			bundle.putString("module", MODULE_NAME);
			bundle.putInt("id", 0); // TODO: adjustable id, multiple modules
			msg.setData(bundle);
			msgSend(msg);

			// TODO: Has to be generated!!
//			Message msg2 = Message.obtain(null, MSG_SET_MESSENGER);
//			msg2.replyTo = mPortInMessgener;
//			Bundle bundle2 = new Bundle();
//			bundle2.putString("module", MODULE_NAME);
//			bundle2.putInt("id", 0);
//			bundle2.putString("port", "in");
//			msg2.setData(bundle2);
//			msgSend(mToMsgService, msg2);
			
			Log.i(TAG, "Connected to MsgService: " + mToMsgService.toString());
		}

		public void onServiceDisconnected(ComponentName className) {
			// This is called when the connection with the service has been unexpectedly disconnected: its process crashed.
			mToMsgService = null;
			Log.i(TAG, "Disconnected from MsgService");
		}
	};

	// Handle messages from MsgService
	class IncomingMsgHandler extends Handler {
		@Override
		public void handleMessage(Message msg) {
			switch (msg.what) {
			case MSG_SET_MESSENGER:
				// TODO: Has to be generated!
//				Log.i(TAG, "set port: " + msg.getData().getString("port") + " to: " + msg.replyTo.toString());
//				if (msg.getData().getString("port").equals("out"))
//					mPortOutMessenger = msg.replyTo;
				break;
			case MSG_STOP:
				Log.i(TAG, "stopping");
				stopSelf();
				break;
			default:
				super.handleMessage(msg);
			}
		}
	}
	
	class PortInMsgHandler extends Handler {
		@Override
		public void handleMessage(Message msg) {
			switch (msg.what) {
			case MSG_PORT_DATA:
				
				// TODO: Has to be generated!
//				float readVal = msg.getData().getFloat("data");
//				Log.i(TAG, "msg: " + readVal);
//				synchronized(mPortOutBuffer) {
//					mPortOutBuffer.add(readVal);
//				}
			default:
				super.handleMessage(msg);
			}
		}
	}
	
	void bindToMsgService() {
		// Establish a connection with the service.  We use an explicit class name because there is no reason to be 
		// able to let other applications replace our component.
		Intent intent = new Intent();
		intent.setClassName("org.dobots.dodedodo", "org.dobots.dodedodo.MsgService");
		bindService(intent, mMsgServiceConnection, Context.BIND_AUTO_CREATE);
		mMsgServiceIsBound = true;
		Log.i(TAG, "Binding to msgService");
	}

	void unbindFromMsgService() {
		if (mMsgServiceIsBound) {
			// If we have received the service, and registered with it, then now is the time to unregister.
			if (mToMsgService != null) {
				Message msg = Message.obtain(null, MSG_UNREGISTER);
				Bundle bundle = new Bundle();
				bundle.putString("module", MODULE_NAME);
				bundle.putInt("id", 0); // TODO: adjustable id, multiple modules
				msg.setData(bundle);
				msgSend(msg);
			}
			// Detach our existing connection.
			unbindService(mMsgServiceConnection);
			mMsgServiceIsBound = false;
			Log.i(TAG, "Unbinding from msgService");
		}
	}

	// Send a msg to the msgService
	protected void msgSend(Message msg) {
		if (!mMsgServiceIsBound) {
			Log.i(TAG, "Can't send message to service: not bound");
			return;
		}
		try {
			msg.replyTo = mFromMsgService;
			mToMsgService.send(msg);
		} catch (RemoteException e) {
			Log.i(TAG, "Failed to send msg to service. " + e);
			// There is nothing special we need to do if the service has crashed.
		}
	}
	
	// Send a msg to some messenger
	protected void msgSend(Messenger messenger, Message msg) {
		if (messenger == null || msg == null)
			return;
		try {
			//msg.replyTo = mFromMsgService;
			messenger.send(msg);
		} catch (RemoteException e) {
			Log.i(TAG, "failed to send msg to service. " + e);
			// There is nothing special we need to do if the service has crashed.
		}
	}

//	class LogStreamer extends Streamer {
//		public void display(String text) {
//			Log.i(TAG, "C++ " + text);
//		}
//	}
	
	// AsyncTask<Params, Progress, Result>
	private class AIMRun extends AsyncTask<Integer, Void, Boolean> {
		protected Boolean doInBackground(Integer... id) {
			Log.i(TAG, "Starting AIMRun");
			int input = 3;
			RotateModule aim = new RotateModule();

//			LogStreamer logStreamer = new LogStreamer();
//			AIM.setStreamer(logStreamer);
			
			// TODO: has to be generated!
			AIMandroidReadPort_t output;
			AIMandroidReadSeqPort_t seqOutput;
			
			vector_float seqInput = new vector_float();
			
			while (true) {
				Log.d(TAG, "mPortOutBuffer=" + mPortOutBuffer);
				synchronized(mPortOutBuffer) {
					if (!mPortOutBuffer.isEmpty()) {
						input = Math.round(mPortOutBuffer.get(0));
						mPortOutBuffer.remove(0);
						aim.androidWritePort(input);
					}
				}
				aim.androidWriteSeqPort(seqInput); // TODO: generated mSeqPortOutBuffer
				
				aim.Tick();
				
				output = aim.androidReadPort();
				if (output.getSuccess()) {
					Log.d(TAG, "Output=" + output.getVal());
					Message msg = Message.obtain(null, MSG_PORT_DATA);
					Bundle bundle = new Bundle();
					bundle.putInt("datatype", DATATYPE_FLOAT);
					bundle.putFloat("data", output.getVal());
					msg.setData(bundle);
					msgSend(mPortOutMessenger, msg);
				}
				
				seqOutput = aim.androidReadSeqPort();
				if (seqOutput.getSuccess()) {
					Log.d(TAG, "seqOutput=" + seqOutput.getVal().toString() + " ");
					for (int i=0; i<seqOutput.getVal().size(); i++) {
						Log.d(TAG, seqOutput.getVal().get(i) + " ");
					}
					// TODO: generated msgSend
				}
				
				if (isCancelled()) break;
			}
			return true;
		}
		
//		protected void onPostExecute() {
//			Log.i(TAG, "Stopped AIMRun");
//		}
		
		protected void onCancelled() {
			Log.i(TAG, "Stopped AIMRun");
		}
	
	}
	
	// static constructor
	static {
		System.loadLibrary("RotateModule");
	}
	
}

