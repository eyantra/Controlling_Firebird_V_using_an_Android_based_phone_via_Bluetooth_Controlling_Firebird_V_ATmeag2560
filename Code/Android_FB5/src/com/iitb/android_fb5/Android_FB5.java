/**
 * Project Name: Android_FB5
 * Author: 	Jatin Kanzaria.
 * 			K.L.Srinivas.
 * 			Rohan Shah.
 * 			Jagbandhu.
 * Date:	8/11/2010
 */
/********************************************************************************

   Copyright (c) 2010, ERTS Lab IIT Bombay erts@cse.iitb.ac.in               -*- c -*-
   All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are met:

   * Redistributions of source code must retain the above copyright
     notice, this list of conditions and the following disclaimer.

   * Redistributions in binary form must reproduce the above copyright
     notice, this list of conditions and the following disclaimer in
     the documentation and/or other materials provided with the
     distribution.

   * Neither the name of the copyright holders nor the names of
     contributors may be used to endorse or promote products derived
     from this software without specific prior written permission.

   * Source code can be used for academic purpose. 
	 For commercial use permission form the author needs to be taken.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
  POSSIBILITY OF SUCH DAMAGE. 

  Software released under Creative Commence cc by-nc-sa licence.
  For legal information refer to: 
  http://creativecommons.org/licenses/by-nc-sa/3.0/legalcode

********************************************************************************/

package com.iitb.android_fb5;

import android.app.Activity;
import android.bluetooth.BluetoothAdapter;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.Toast;

/** Main activity which starts when the application is first Run.
 * Task: (1)Initialise the User Interface.
 * 		 (2)Initialise button click listeners.
 * 		 (3)Implements all button click listeners.
 * 		 (4)Call subroutines to connect to Bluetooth and start Accelerometer on 'Connect' button click.
 * 		 (5)Call subroutines to disconnect from bluetooth and stop accelerometer on close of application.
 */
public class Android_FB5 extends Activity {

	final String tag = "Android_FB5";

	/** Bluetooth related objects. */
	private AccelerometerReader mAccelerometerReader = null;
	private BluetoothComm mBluetoothComm = null;
	private static final int REQUEST_ENABLE_BT = 2;
	private BluetoothAdapter mAdapter = null;
	
	/** UI related objects. */
	private Button mSendButton;
	private Button mConnectButton;
	private Button mDisconnectButton;
	private ImageView mImageView;
	
	/** Called when the activity is first created. 
	 *  Starts all the button click listeners.*/
	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		
		setContentView(R.layout.main); /** Set the layout of UI from "/res/layout/main". */
		
		Log.d(tag,"Android FB5 started..");

		mAdapter = BluetoothAdapter.getDefaultAdapter(); /** Get the Bluetooth hardware and create a handle for it. */
		if (mAdapter == null) {
			Toast.makeText(this, "Bluetooth is not available. Closing Application", Toast.LENGTH_LONG).show();
			finish();
			return;
		}

		mSendButton = (Button) findViewById(R.id.button_send); /** Start listeners for button click.*/
		mSendButton.setOnClickListener(SendListener);
		
		mConnectButton = (Button)this.findViewById(R.id.connect);
		mConnectButton.setOnClickListener(ConnectListener);      
		
		mDisconnectButton = (Button)this.findViewById(R.id.disconnect);
		mDisconnectButton.setOnClickListener(DisconnectListener);      
		
		mDisconnectButton.setEnabled(false); /** Enable only 'Connect' button initially. */
		mConnectButton.setEnabled(true);
		mSendButton.setEnabled(false);
		
		mImageView = (ImageView) findViewById(R.id.ImageView);
		mImageView.setImageBitmap(null);
	}


	/** Called when 'Connect' button is clicked. Starts the connection procedure over BT*/
	private OnClickListener ConnectListener = new OnClickListener()  
	{  
		public void onClick(View v)  
		{         
			Log.d(tag,"Connect Requested");    
			startup();  /** Start the BT connection process and accelerometer.*/ 
			
		}  
	};

	/** Called when 'Disconnect' button is pressed. Frees the BT channel and stop accelerometer listener. */
	private OnClickListener DisconnectListener = new OnClickListener()  
	{  
		public void onClick(View v)  
		{         
			Log.d(tag,"Disonnect Requested");    
			if(mBluetoothComm != null)mBluetoothComm.free_channel(); /**Free up the BT channel. */
			if(mAccelerometerReader != null) mAccelerometerReader.unregisterListener();  /** Stop listening to Accelerometer changes. */
			mConnectButton.setEnabled(true);
			mDisconnectButton.setEnabled(false);
			mSendButton.setEnabled(false);
		}    
	};

	/** Only used for testing purpose. */
	private OnClickListener SendListener = new OnClickListener() {
		public void onClick(View v) {

			// sending test string...
			byte[] write_buffer = new byte[6];
			write_buffer[0] = 'J';
			write_buffer[1] = 'A';
			write_buffer[2] = 'T';
			write_buffer[3] = 'I';
			write_buffer[4] = 'N';

			try {mBluetoothComm.BluetoothSend(write_buffer);   
			}
			catch (Exception e){e.printStackTrace();
			}
			Log.d(tag, "Write on button press successful");  

		}
	};	
	
	/** Called when the activity starts. Gives a request to turn ON the Bluetooth id OFF*/
	@Override
	public void onStart() {
		super.onStart();
		Log.d(tag, "++ ON START ++");
		/** If bluetooth is not enabled, ask for user permission to turn on bluetooth. */
		if (!mAdapter.isEnabled()) {
			Intent enableIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
			startActivityForResult(enableIntent, REQUEST_ENABLE_BT);
		} else {
			//if(mAccelerometerReader == null) startup();
		}
	}



	/** Called when the activity resumes. */
	@Override
	public synchronized void onResume() {
		super.onResume();
		if(mAccelerometerReader != null){
			mAccelerometerReader.registerListener();
			}
	}
	

	/** Called when the activity is aborted. 
	 * Stops the BT channel and stops accelerometer listener. */
	@Override
	public void onDestroy() {
		super.onDestroy();
		if(mAccelerometerReader != null){
			mAccelerometerReader.unregisterListener();}
		if(mBluetoothComm != null){
			mBluetoothComm.free_channel();}
		Log.e(tag, "--- ON DESTROY ---");
	}

	/** Initialisation function.
	 * Called from : 'Connect' button click listener.
	 * Task: (1)Establish connection between phone and bluetooth module on FB5. 
	 * 		 (2)Start a listener for changes in value of Accelerometer sensor.
	 * Arguments : Null 
	 * Return : Null
	 */
	private void startup()
	{
		mBluetoothComm = new BluetoothComm(this);
		Toast.makeText(this, "Connecting...", Toast.LENGTH_LONG).show();
		try {
			Log.d(tag, "Initialisation Started...");
			
			/** Bluetooth initialise function returns true if connection is succesful, else false. */
			if(mBluetoothComm.Initialise() == false) 
			{
				Toast.makeText(this, " No connection established ", Toast.LENGTH_SHORT).show();
				return;
			}
			else 
			{
				Toast.makeText(this, " Connection established ", Toast.LENGTH_SHORT).show();
			}
			Log.d(tag, "Initialisation Successful");
		} catch (Exception e) {
			e.printStackTrace();
			Log.e(tag, "Initialisation Failed");
		}
		/** Accelerometer initialisation. */
		mAccelerometerReader = new AccelerometerReader(getApplicationContext(),this,mBluetoothComm);
		/** Enable 'Disconnect' button. */
		mDisconnectButton.setEnabled(true);
		mConnectButton.setEnabled(false);
		mSendButton.setEnabled(true);
	}

	/** Called when the activity resumes after prompting user to turn ON the bluetooth. 
	 * If turned ON, goes ahead with application, else closes the connection and stops application.
	 */
	public void onActivityResult(int requestCode, int resultCode, Intent data)
	{
		Log.d(tag, "onActivityResult " + resultCode);
		if (requestCode == REQUEST_ENABLE_BT) 
		{
			/** When the request to enable Bluetooth returns. */
			if (resultCode == Activity.RESULT_OK) {
				Log.d(tag,"BT Enabled");
				// Bluetooth is now enabled
			} else {
				// User did not enable Bluetooth or an error occured
				Log.d(tag, "BT not enabled");
				Toast.makeText(this, "Bluetooth was not enabled. Closing application..", Toast.LENGTH_LONG).show();
				finish();  /** Terminate the activity and close application. */
				return;
			}
		}
	}

}