package com.chendong.debugtools;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;

public class ToolsFileOps {

	public static String getFileCxt(String path) throws IOException {
		FileInputStream fis = new FileInputStream(path);
		int len = fis.available();
		byte[] bytes = new byte[len];
		fis.read(bytes);
		fis.close();
		return new String(bytes);
	}

	public static void setFileCxt(String path, String str)
			throws IOException {
		
        File f = new File(path);
  //     FileOutputStream fout = new FileOutputStream(file2);  
		FileOutputStream fos = new FileOutputStream(f);
		byte[] sendbytes=str.getBytes("UTF8");
		
		fos.write(sendbytes);
		fos.close();
	}

	public static StringBuffer execCommand(String command) throws IOException {
		StringBuffer stringBuffer = new StringBuffer();
		;
		Runtime runtime = Runtime.getRuntime();
		Process proc = runtime.exec(command);
		try {
			if (proc.waitFor() != 0) {
				System.err.println("exit value = " + proc.exitValue());
			}
			BufferedReader in = new BufferedReader(new InputStreamReader(
					proc.getInputStream()));

			String line = null;
			while ((line = in.readLine()) != null) {
				stringBuffer.append(line + "\n");
			}
			System.out.println(stringBuffer.toString());

		} catch (InterruptedException e) {
			System.err.println(e);
		}
		return stringBuffer;
	}
}
