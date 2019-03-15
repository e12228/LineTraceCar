//www.elegoo.com

/*
//Line Tracking IO define
#define LT_R !digitalRead(10)
#define LT_M !digitalRead(4)
#define LT_L !digitalRead(2)

#define ENA 5
#define ENB 6
#define IN1 7
#define IN2 8
#define IN3 9
#define IN4 11
*/


#define LT_R (!digitalRead(39))
#define LT_M (!digitalRead(17))
#define LT_L (!digitalRead(26))

#define ENA 16
#define ENB 27
#define IN1 14
#define IN2 12
#define IN3 13
#define IN4 23

// LED
#define	IO_PIN_LED				(18)
#define	IO_PIN_LED2				(2)

#define carSpeed 150

enum {
	TRACK_EVENT_XXX	= 0,
	TRACK_EVENT_XXO,
	TRACK_EVENT_XOX,
	TRACK_EVENT_XOO,
	TRACK_EVENT_OXX,
	TRACK_EVENT_OXO,
	TRACK_EVENT_OOX,
	TRACK_EVENT_OOO,

	TRACK_EVENT_NUM
};

enum {
	STATE_TURN_LEFT = 0,	
	STATE_GO_FORWARD,	
	STATE_TURN_RIGHT,	

	STATE_NUM	
};

int g_next_event_table[TRACK_EVENT_NUM][STATE_NUM] =
{
	STATE_TURN_LEFT,	STATE_GO_FORWARD,	STATE_TURN_RIGHT,
	STATE_TURN_RIGHT,	STATE_TURN_RIGHT,	STATE_TURN_RIGHT,
	STATE_GO_FORWARD,	STATE_GO_FORWARD,	STATE_GO_FORWARD,
	STATE_GO_FORWARD,	STATE_GO_FORWARD,	STATE_TURN_RIGHT,
	STATE_TURN_LEFT,	STATE_TURN_LEFT,	STATE_TURN_LEFT,
	STATE_TURN_LEFT,	STATE_TURN_RIGHT,	STATE_TURN_RIGHT,
	STATE_TURN_LEFT,	STATE_GO_FORWARD,	STATE_GO_FORWARD,
	STATE_TURN_LEFT,	STATE_GO_FORWARD,	STATE_TURN_RIGHT,
};

void forward()
{
	ledcWrite(0, carSpeed);  
	ledcWrite(1, carSpeed);  
	digitalWrite(IN1, HIGH);
	digitalWrite(IN2, LOW);
	digitalWrite(IN3, LOW);
	digitalWrite(IN4, HIGH);
	Serial.println("go forward!");
}

void back(){
	ledcWrite(0, carSpeed);  
	ledcWrite(1, carSpeed);  
//  analogWrite(ENA, carSpeed);
//  analogWrite(ENB, carSpeed);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  Serial.println("go back!");
}

void left(){
	ledcWrite(0, carSpeed);  
	ledcWrite(1, carSpeed);  
//  analogWrite(ENA, carSpeed);
//  analogWrite(ENB, carSpeed);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  Serial.println("go left!");
}

void right(){
	ledcWrite(0, carSpeed);  
	ledcWrite(1, carSpeed);  
//  analogWrite(ENA, carSpeed);
//  analogWrite(ENB, carSpeed);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW); 
  Serial.println("go right!");
} 

void stop(){
   digitalWrite(ENA, LOW);
   digitalWrite(ENB, LOW);
   Serial.println("Stop!");
} 

int	g_cur_state = STATE_GO_FORWARD;
void setup()
{
	Serial.begin(9600);
	pinMode(39,INPUT);
	pinMode(17,INPUT);
	pinMode(26,INPUT);
	pinMode(IO_PIN_LED,OUTPUT);
	pinMode(ENA,OUTPUT);
	pinMode(ENB,OUTPUT);
	pinMode(IN1,OUTPUT);
	pinMode(IN2,OUTPUT);
	pinMode(IN3,OUTPUT);
	pinMode(IN4,OUTPUT);

	ledcSetup(0, 980, 8);
	ledcSetup(1, 980, 8);
	ledcAttachPin(ENA, 0);
	ledcAttachPin(ENB, 1);

	forward();
	g_cur_state = STATE_GO_FORWARD;

	digitalWrite(IO_PIN_LED,HIGH);
}

void loop() 
{
/*
	Serial.print("");
	Serial.print(LT_L);
	Serial.print("");
	Serial.print(LT_M);
	Serial.print("");
	Serial.print(LT_R);
	Serial.println();
	delay(500);	
*/
	int event = ((LT_L&0x1)<<2) | ((LT_M&0x1)<<1) | ((LT_R&0x1)<<0);
	int next_state = g_next_event_table[event][g_cur_state];
	
	if(next_state != g_cur_state) {
		if(next_state == STATE_TURN_LEFT) {
			left();
		}
		else if(next_state == STATE_GO_FORWARD) {
			forward();
		}
		else if(next_state == STATE_TURN_RIGHT) {
			right();
		}
		
		g_cur_state = next_state;
	}
}

