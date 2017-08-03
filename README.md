# SignalAndSystem_Special_Topic

因為檔案很少就沒有寫makefile了XD，可以自行compile執行

<table>

<tr valign="top"><td>
	<a href="https://github.com/yoyololicon/SignalAndSystem_Special_Topic/tree/master/origin%20data">origin data</a>
  
## /origin data: 
我自己的心電圖、脈搏、胸腹呼吸訊號資料。You can use it for free。   

## FFT.cpp: 
*program.o RRI.txt 資料時間長度*   
power spectrum會儲存在輸入RRI_psd.txt裡，格式為"頻率 power數值"。   
output為"lf 低頻功率; hf 高頻功率"   

## RR_interval.cpp: 
*program.o 輸入資料.txt [threshold]*  
會抓輸入資料的RR interval再輸出到螢幕上，格式為"時間 interval"，如要儲存請在後面加上 *> output.txt*   
可用在ECG與BloodPulse上  
**※threshold預設為0.55**   

## amp_freq.cpp: 
*program.o 輸入資料.txt [confidence]*   
呼吸頻率的變化會儲存在輸入資料_freq.txt裡   
output為"average freq: 平均頻率, average amp: 平均震幅"   
**※手動調整confidence來修改準確度，預設為250 sample**

## csv_to_txt_convert.cpp:
*program.o 輸入資料.csv [sign]*   
