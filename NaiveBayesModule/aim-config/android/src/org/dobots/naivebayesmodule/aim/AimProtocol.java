package org.dobots.naivebayesmodule.aim;

public final class AimProtocol {
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
	public static final int MSG_NOT_INSTALLED = 15;
	public static final int MSG_XMPP_MSG = 16;
	
	public static final int DATATYPE_FLOAT = 1;
	public static final int DATATYPE_FLOAT_ARRAY = 2;
	public static final int DATATYPE_INT = 3;
	public static final int DATATYPE_INT_ARRAY = 4;
	public static final int DATATYPE_STRING = 5;
	public static final int DATATYPE_IMAGE = 6;
	public static final int DATATYPE_BINARY = 7;
	
	public static int getDataType(String s) {
		int res = 0;
		if (s.equals("float"))
			res = DATATYPE_FLOAT;
		else if (s.equals("floatarray"))
			res = DATATYPE_FLOAT_ARRAY;
		else if (s.equals("int"))
			res = DATATYPE_INT;
		else if (s.equals("intarray"))
			res = DATATYPE_INT_ARRAY;
		else if (s.equals("string"))
			res = DATATYPE_STRING;
		else if (s.equals("image"))
			res = DATATYPE_IMAGE;
		else if (s.equals("binary"))
			res = DATATYPE_BINARY;
		return res;
	}
	
	public static String getDataType(int t) {
		String res = null;
		switch (t) {
		case DATATYPE_FLOAT:
			res = new String("float"); break;
		case DATATYPE_FLOAT_ARRAY:
			res = new String("floatarray"); break;
		case DATATYPE_INT:
			res = new String("int"); break;
		case DATATYPE_INT_ARRAY:
			res = new String("intarray"); break;
		case DATATYPE_STRING:
			res = new String("string"); break;
		case DATATYPE_IMAGE:
			res = new String("image"); break;
		case DATATYPE_BINARY:
			res = new String("binary"); break;
		}
		return res;
	}
}
