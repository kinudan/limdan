//===  전면, 후진, 수평 주차   


unsigned char FL1 = 0;	 
byte KeyInput = 0;

void setup()
{Serial.begin(9600); 
 
  DDRB = 0B11101000;		//아두이노 포트 제어
  PORTB =0B00000111;

  DDRC = 0B00011000;		//아두이노 포트 제어
  PORTC =0B11000000;
 
  DDRD = 0B11111110;		//아두이노 포트 제어
  PORTD =0B00000011; 
}

void TRIG(void)				//초음파 트리거 
{digitalWrite(11,1);
 delayMicroseconds(10);
 digitalWrite(11,0);    
} 

void FRONT(void)			//전진 
{digitalWrite(7,1); digitalWrite(6,0); 
 digitalWrite(5,1); digitalWrite(4,0);} 

void STOP(void)				//정지 
{digitalWrite(7,0); digitalWrite(6,0); 
 digitalWrite(5,0); digitalWrite(4,0);} 

void BACK(void)				//후진 
{digitalWrite(7,0); digitalWrite(6,1); 
 digitalWrite(5,0); digitalWrite(4,1);} 


void CAR_Y2N(void)			 //벽감지  
{do {delay(50); TRIG(); delay(2);}     
     while(!(digitalRead(12)));	 	 
}

void CAR_N2Y2(void)			   //벽감지하고 거리측정 
{do {delay(50); TRIG(); delay(2); FL1++;} //거리측정.
     while((digitalRead(12)));	 	   
}

void CAR_CHK(void)	   	//2개의 벽감지 함수 
{ FRONT(); delay(500);		//일단전진
 CAR_Y2N();			//1번째 벽 
 delay(500);			//0.5초후 거리측정시작.
 CAR_N2Y2();			//2번째 벽 
} 
    

void SENSOR(void)		   //라인센서 체크 함수
{if(digitalRead(A2))		   //왼쪽센서 감지시. 
  {digitalWrite(5,0); delay(10);}  //왼쪽 모터 일시 정지
 if(digitalRead(A1))	           //오른쪽센서 감지시 
  {digitalWrite(7,0); delay(10);}  //오른쪽 모터 일시 정지  
}   


void loop()
{ if(Serial.available() > 0)     //핸폰으로부터 데이타수신. 
   KeyInput = Serial.read();     
   if(KeyInput=='1') {KeyInput=0; goto LOP;}  //라인트레이서
   if(KeyInput=='2') {KeyInput=0; goto LOP2;} //후진/수평.
   if(KeyInput=='3') {KeyInput=0; goto LOP3;} //전면/수평.     


//===== 라인 트레이서 모드  ====================
    	
 if(!(digitalRead(8)))		//버튼 누름.
  {
LOP:  
   if(!(digitalRead(A0))) {STOP(); goto LOP;} //장애물 있으면 정지.		 
   FRONT(); SENSOR(); goto LOP;} 	      //없으면 전진.	
	
 
//===== 후진/수평 모드 =========================  
 if(!(digitalRead(9)))		//버튼 누르면.....
   {delay(1000);		//잠시후 
LOP2:   
         
    CAR_CHK();			//출발및 차감지.
    STOP(); 
   
    if(FL1<18)			//거리 측정해서 좁으면 후진주차.
     {digitalWrite(7,1); delay(500);//핸들 좌로.
      STOP(); delay(1000);
      BACK(); delay(600);	//후진하고 정지.
      STOP();  
      digitalWrite(4,1); delay(700);//핸들 우로
      STOP();
      BACK(); delay(800);    	//진입.
      STOP();  	                //종료      
     }
   
    else 			//넓으면 수평주차.
     {
HORIZ:       
      digitalWrite(7,1); delay(600);//핸들 좌로.
      STOP(); delay(1000);	    // 
      BACK(); delay(1400);	    //후진하고 정지.
      STOP();  
      digitalWrite(6,1); delay(400);//핸들 좌로
      STOP(); delay(1000); 
      STOP(); 			    //종료 
     }    
   }

//======== 전면/수평 모드 ================================ 
 if(!(digitalRead(10)))		//전면/수평 버튼 누르면.....
   {delay(1000);		//잠시후    
LOP3:   
    CAR_CHK();			//출발및 차감지.
    STOP(); 
   
    if(FL1<18)			//거리 측정해서 좁으면 전면주차 
     {BACK(); delay(300);	//후진하고 정지.
      STOP();
      digitalWrite(6,1); delay(1000);//핸들 우로.
      STOP(); delay(1000);
      FRONT(); delay(1000);	// 진하고     
      STOP();           	//종료      
     }
   
    else goto HORIZ;		 //넓으면 수평주차.
   } 
}
 
