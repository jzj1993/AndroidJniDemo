package com.demo.jnidemo;

import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;

import androidx.appcompat.app.AppCompatActivity;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    private ViewGroup mContainer;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        mContainer = findViewById(R.id.container);
        for (int i = 1; i <= 8; i++) {
            final int finalI = i;
            final String text = i + "";
            Button button = new Button(this);
            button.setText(text);
            button.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View v) {
                    try {
                        Log.d("Java", text + " before");
                        nativeMethod(finalI);
                        Log.d("Java", text + " end");
                    } catch (Throwable t) {
                        Log.e("Java", text + " error", t);
                    }
                }
            });
            mContainer.addView(button);
        }
    }

    void normalMethod() {
        Log.d("Java", "normal method");
    }

    void exceptionMethod() {
        throw new RuntimeException("java runtime exception");
    }

    public native void nativeMethod(int i);
}
