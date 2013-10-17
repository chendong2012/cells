package com.debug.sensordebug;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;

public class ToolsFileOps {

	public static int global_switch = 0;
	public static final String data128to158_table[] =
	{
		"128","129",
        "130", "131", "132","133", "134", "135","136", "137", "138","139",
        "140", "141", "142","143", "144", "145","146", "147", "148","149",
        "150", "151", "152","153", "154", "155","156", "157", "158",
	};
	public static final String data128to158_table1[] =
	{
		"128(0x80)","129(0x81)", "130(0x82)", "131(0x83)", 
		"132(0x84)","133(0x85)", "134(0x86)", "135(0x87)",
		"136(0x88)", "137(0x89)", "138(0x8A)","139(0x8B)",
		"140(0x8C)", "141(0x8D)", "142(0x8E)","143(0x8F)", "144(0x90)",
		"145(0x91)","146(0x92)", "147(0x93)", "148(0x94)","149(0x95)",
      "150(0x96)", "151(0x97)", "152(0x98)","153(0x99)", "154(0x9A)", 
      "155(0x9B)","156(0x9C)", "157(0x9D)", "158(0x9E)",
	};
	public static final String data128to158_table2[] =
	{
        "0x80","0x81", "0x82", "0x83",        
        "0x84","0x85", "0x86", "0x87",
        "0x88", "0x89", "0x8A","0x8B",
        "0x8C", "0x8D", "0x8E","0x8F", "0x90",
        "0x91","0x92", "0x93", "0x94","0x95",
"0x96", "0x97", "0x98","0x99", "0x9A",          
"0x9B","0x9C", "0x9D", "0x9E",
	};
/*als setting*/	
	public  static final String als_gain_table[]= {
		 "1", "2", "4", "8", "48", "96",
	};
	
    public static final String als_persist_table[] = {
        "0", "1", "2","3", "4", "5","6", "7", "8","9", 
        "10", "11", "12","13", "14", "15",
        };	
    
    public static final String als_integration_time_table[] = {
    	"100", "50", "200", "400", "150", "250", "300", "350",
    };

    public static final String als_meas_repeat_rate_table[] = {
    	"50", "100", "200", "500", "1000", "2000"
    };

    public static final String led_pulse_freq_table[] = {
        "30", "40", "50", "60", "70", "80", "90", "100"
};
/*als setting end*/
	
/*ps setting*/
	public static final String ps_gain_table[] = { "16", "32", "64" };

	public static final String ps_measurement_time_table[] = { "50", "70",
			"100", "200", "500", "1000", "2000", };

	public static final String ps_persist_table[] = { "0", "1", "2", "3", "4", "5",
			"6", "7", "8", "9", "10", "11", "12", "13", "14", "15", };

	// 10、PS Pulse Count: 0~255

	public static final String led_driving_peak_count_table[] = { "5", "10", "20",
			"50", "100", };

	public static final String led_duty_cycle_table[] = { "25", "50", "75", "100" };

/*ps setting end*/
	
    public static final String data0to255_table[] = {
        "0", "1", "2","3", "4", "5","6", "7", "8","9", 
        "10", "11", "12","13", "14", "15","16", "17", "18","19",
        "20", "21", "22","23", "24", "25","26", "27", "28","29",
        "30", "31", "32","33", "34", "35","36", "37", "38","39",
        "40", "41", "42","43", "44", "45","46", "47", "48","49",
        "50", "51", "52","53", "54", "55","56", "57", "58","59",
        "60", "61", "62","63", "64", "65","66", "67", "68","69",
        "70", "71", "72","73", "74", "75","76", "77", "78","79",
        "80", "81", "82","83", "84", "85","86", "87", "88","89",
        "90", "91", "92","93", "94", "95","96", "97", "98","99",

        "100", "101", "102","103", "104", "105","106", "107", "108","109",
        "110", "111", "112","113", "114", "115","116", "117", "118","119",
        "120", "121", "122","123", "124", "125","126", "127", "128","129",
        "130", "131", "132","133", "134", "135","136", "137", "138","139",
        "140", "141", "142","143", "144", "145","146", "147", "148","149",
        "150", "151", "152","153", "154", "155","156", "157", "158","159",
        "160", "161", "162","163", "164", "165","166", "167", "168","169",
        "170", "171", "172","173", "174", "175","176", "177", "178","179",
        "180", "181", "182","183", "184", "185","186", "187", "188","189",
        "190", "191", "192","193", "194", "195","196", "197", "198","199",

        "200", "201", "202","203", "204", "205","206", "207", "208","209",
        "210", "211", "212","213", "214", "215","216", "217", "218","219",
        "220", "221", "222","223", "224", "225","226", "227", "228","229",
        "230", "231", "232","233", "234", "235","236", "237", "238","239",
        "240", "241", "242","243", "244", "245","246", "247", "248","249",
        "250", "251", "252","253", "254", "255",

    };

    /* 描述：从指定的文件里面，获取数据
     * 参数：String path:是完整路径
     * 返回值：从文件里面读回来的字符串内容。
     * */
    
	public static String getFileCxt(String path) throws IOException {
		FileInputStream fis = new FileInputStream(path);
		int len = fis.available();
		byte[] bytes = new byte[len];
		fis.read(bytes);
		fis.close();
		return new String(bytes);
	}

    /* 描述：指定的文件，写入数据
     * 参数：String path:是完整路径
    * String str:要写入的字符串
     * 返回值：从文件里面读回来的字符串内容。
     * */
	
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
