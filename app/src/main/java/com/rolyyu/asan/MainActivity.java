package com.rolyyu.asan;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.view.View;
import android.widget.TextView;

import com.rolyyu.asan.databinding.ActivityMainBinding;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'asan' library on application startup.
    static {
        System.loadLibrary("asan");
    }

    private ActivityMainBinding binding;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        binding = ActivityMainBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());

        binding.buttonHeapUseAfterFree.setOnClickListener(v -> doHeapUseAfterFree());

        binding.buttonHeapBufferOverflow.setOnClickListener(v -> doHeapBufferOverflow());

        binding.buttonStackBufferOverflow.setOnClickListener(v -> doStackBufferOverflow());

        binding.buttonStackBufferUnderflow.setOnClickListener(v -> doStackBufferUnderflow());

        binding.buttonStackUseAfterReturn.setOnClickListener(v -> doStackUseAfterReturn());

        binding.buttonDoubleFree.setOnClickListener(v -> doDoubleFree());

        binding.buttonDoNullPointerDereference.setOnClickListener(v -> doNullpointerDereference());

        binding.buttonMemcpyParamOverlap.setOnClickListener(v -> doMemcpyParamOverlap());

        binding.buttonStrncatParamOverlap.setOnClickListener(v -> doStrncatParamOverlap());
    }

    public native void doHeapUseAfterFree();
    public native void doHeapBufferOverflow();
    public native void doStackBufferOverflow();
    public native void doStackBufferUnderflow();
    public native void doStackUseAfterReturn();
    public native void doDoubleFree();
    public native void doNullpointerDereference();
    public native void doMemcpyParamOverlap();
    public native void doStrncatParamOverlap();

}