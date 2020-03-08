package co.fakhreddin.cube;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;

public class MainActivity extends AppCompatActivity {

    GL2JNIView mView;

    private static MainActivity instance;

    public static MainActivity getInstance(){
        return instance;
    }

    @Override protected void onCreate(Bundle icicle) {
        super.onCreate(icicle);
        instance = this;
        mView = new GL2JNIView(getApplication());
        setContentView(mView);
    }

    @Override protected void onPause() {
        super.onPause();
        mView.onPause();
    }

    @Override protected void onResume() {
        super.onResume();
        mView.onResume();
    }
}
