package com.qiyi.androidjnidemo;

import android.graphics.SurfaceTexture;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.widget.TextView;

import java.util.Date;

public class MainActivity extends AppCompatActivity {
    private static final String TAG = "JNI_DEMO";

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    public int[] globalIntAry = {21,22,23};

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // Example of a call to a native method
        TextView tv = (TextView) findViewById(R.id.sample_text);
        tv.setText(stringFromJNI());

        //传递string到底层
        callJniArgStr("Hello 中国！");
        //传递int
        callJniArgInt(100);

        callJniArgChar('p');

        int[] iArray = {10,11,12};
        callJniArgIntAry(iArray);

        callJniGetJavaInrAry();
    }

    public int property;
    public int function(int i, Date date,int[] ary){
        Log.d(TAG," java client call function");
        if(null != ary){
            for(int j = 0; j < ary.length; j++ ){
                Log.d(TAG," java client call function ary["+j+"]:"+ary[j]);
            }
        }
        Log.d(TAG," java client call function end");
        return 1;
    }

    public void function(char c){
        Log.d(TAG," java client call function c :"+c);
    }

    public void functionString(String str){
        Log.d(TAG," java client call functionString str :"+str);
    }

    {

    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     * 非空调用
     */
    public native String stringFromJNI();

    //传递string到jni层
    public native void callJniArgStr(String str);

    //传递int到jni层
    public native void callJniArgInt(int i);

    //传递char到jni层
    public native void callJniArgChar(char c);

    //传递int[]到jni层
    public native void callJniArgIntAry(int[] array);

    public native void callJniGetJavaInrAry();
}
