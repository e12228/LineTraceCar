////////////////////////////////////////////////////////////////////////////////
//
//		LineTrace_lab_04.ino
//			
//			State Machine��p����LineTrace����
//			�������������o����ƒ�Ԃ���
//
////////////////////////////////////////////////////////////////////////////////


// Motor Pin
#define	IO_PIN_MOTOR_1			(14)
#define	IO_PIN_MOTOR_2			(12)
#define	IO_PIN_MOTOR_3			(13)
#define	IO_PIN_MOTOR_4			(23)
#define	IO_PIN_MOTOR_ENA		(16)
#define	IO_PIN_MOTOR_ENB		(27)
// Line Tracking Sensor Pin
#define IO_PIN_LINETRACK_LEFT	(26)
#define IO_PIN_LINETRACK_CENTER	(17)
#define IO_PIN_LINETRACK_RIGHT	(39)

// Motor Speed
#define	MOTOR_SPEED				(150)

// State Machine��Event
enum {
	TRACK_EVENT_NONE = -1,
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

// State Machine��State
enum {
	STATE_ROTATO_LEFT = 0,	
	STATE_GO_FORWARD,	
	STATE_ROTATO_RIGHT,	
	STATE_STOP,	

	STATE_NUM	
};

// ��ԑJ�ڃe�[�u��
int g_next_event_table[TRACK_EVENT_NUM][STATE_NUM] =
{
//				Left				Center				Right				Stop
/*XXX*/		STATE_ROTATO_LEFT,	STATE_ROTATO_LEFT,	STATE_ROTATO_RIGHT,	STATE_GO_FORWARD,
/*XXO*/		STATE_ROTATO_RIGHT,	STATE_ROTATO_RIGHT,	STATE_ROTATO_RIGHT,	STATE_STOP,
/*XOX*/		STATE_GO_FORWARD,	STATE_GO_FORWARD,	STATE_GO_FORWARD,	STATE_STOP,
/*XOO*/		STATE_GO_FORWARD,	STATE_GO_FORWARD,	STATE_ROTATO_RIGHT,	STATE_STOP,	
/*OXX*/		STATE_ROTATO_LEFT,	STATE_ROTATO_LEFT,	STATE_ROTATO_LEFT,	STATE_STOP,
/*OXO*/		STATE_ROTATO_LEFT,	STATE_ROTATO_RIGHT,	STATE_ROTATO_RIGHT,	STATE_STOP,	
/*OOX*/		STATE_ROTATO_LEFT,	STATE_GO_FORWARD,	STATE_GO_FORWARD,	STATE_STOP,	
/*OOO*/		STATE_STOP,			STATE_STOP,			STATE_STOP,			STATE_STOP,
};

// �����
int g_cur_state;



/////////////////////////////////////////////////////////////////////
// MODULE   : read_sensor_*()
// FUNCTION : �Z���T�[��ǂ�(Left/Center/Right)
// RETURN   : �Ȃ�
/////////////////////////////////////////////////////////////////////
int read_sensor_L()
{
	return	!digitalRead(IO_PIN_LINETRACK_LEFT);
}
int read_sensor_C()
{
	return	!digitalRead(IO_PIN_LINETRACK_CENTER);
}
int read_sensor_R()
{
	return	!digitalRead(IO_PIN_LINETRACK_RIGHT);
}

/////////////////////////////////////////////////////////////////////
// MODULE   : forward()
// FUNCTION : ���i
// RETURN   : �Ȃ�
/////////////////////////////////////////////////////////////////////
void forward()
{
	ledcWrite(0, MOTOR_SPEED);  
	ledcWrite(1, MOTOR_SPEED);  
	digitalWrite(IO_PIN_MOTOR_1, HIGH);
	digitalWrite(IO_PIN_MOTOR_2, LOW);
	digitalWrite(IO_PIN_MOTOR_3, LOW);
	digitalWrite(IO_PIN_MOTOR_4, HIGH);
	
	g_cur_state = STATE_GO_FORWARD;
	
	Serial.println("go forward!");
}

/////////////////////////////////////////////////////////////////////
// MODULE   : left()
// FUNCTION : ����]
// RETURN   : �Ȃ�
/////////////////////////////////////////////////////////////////////
void left()
{
	ledcWrite(0, MOTOR_SPEED);  
	ledcWrite(1, MOTOR_SPEED);  
	digitalWrite(IO_PIN_MOTOR_1, LOW);
	digitalWrite(IO_PIN_MOTOR_2, HIGH);
	digitalWrite(IO_PIN_MOTOR_3, LOW);
	digitalWrite(IO_PIN_MOTOR_4, HIGH);

	g_cur_state = STATE_ROTATO_LEFT;

	Serial.println("go left!");
}

/////////////////////////////////////////////////////////////////////
// MODULE   : right()
// FUNCTION : �E��]
// RETURN   : �Ȃ�
/////////////////////////////////////////////////////////////////////
void right()
{
	ledcWrite(0, MOTOR_SPEED);  
	ledcWrite(1, MOTOR_SPEED);  
	digitalWrite(IO_PIN_MOTOR_1, HIGH);
	digitalWrite(IO_PIN_MOTOR_2, LOW);
	digitalWrite(IO_PIN_MOTOR_3, HIGH);
	digitalWrite(IO_PIN_MOTOR_4, LOW); 

	g_cur_state = STATE_ROTATO_RIGHT;

	Serial.println("go right!");
} 

/////////////////////////////////////////////////////////////////////
// MODULE   : stop()
// FUNCTION : ��~
// RETURN   : �Ȃ�
/////////////////////////////////////////////////////////////////////
void stop()
{
	ledcWrite(0, 0);  
	ledcWrite(1, 0);  

	g_cur_state = STATE_STOP;

	Serial.println("Stop!");
} 

/////////////////////////////////////////////////////////////////////
// MODULE   : create_event()
// FUNCTION : State Machine�̓���Event����
// RETURN   : Event
/////////////////////////////////////////////////////////////////////
int create_event()
{
	int sensor_L = read_sensor_L();
	int sensor_C = read_sensor_C();
	int sensor_R = read_sensor_R();
	
	int event = ((sensor_L&0x1)<<2) | ((sensor_C&0x1)<<1) | ((sensor_R&0x1)<<0);
	
	return	event;
}

/////////////////////////////////////////////////////////////////////
// MODULE   : get_next_state()
// FUNCTION : State Machine��event�ɑ΂���J�ڌ�̏�Ԃ��擾
// RETURN   : Event
/////////////////////////////////////////////////////////////////////
int get_next_state(int event) // event : State Machine�ɔ��������C�x���g
{
	if((event < 0) || (TRACK_EVENT_NUM <= event)) {
		// �ϐ�event�̒l���s��
		Serial.println("[Error] Event id is out of range.");
		return	TRACK_EVENT_NONE;
	}
	
	if((g_cur_state < 0) || (STATE_NUM <= g_cur_state)) {
		// �ϐ�g_cur_state�̒l���s��
		Serial.println("[Error] Invalid state.");
		return	TRACK_EVENT_NONE;
	}
	
	return	g_next_event_table[event][g_cur_state];
}

/////////////////////////////////////////////////////////////////////
// MODULE   : Task_line_trace()
// FUNCTION : [Task] ���C���g���[�X�J�[����
// RETURN   : �Ȃ�
/////////////////////////////////////////////////////////////////////
void Task_line_trace(void* param)
{
	for(;;) {
		vTaskDelay(10);

		// �C�x���g����
		int event = create_event();
		if(event != -1) { // �s���C�x���g�`�F�b�N
			// ����Ԏ擾
			int next_state = get_next_state(event);
			
			if(next_state != g_cur_state) { // ����Ԃ����݂̏�ԂƈقȂ� => ��ԑJ�ڔ���
				// ��Ԃɉ���������
				if(next_state == STATE_ROTATO_LEFT) {
					left();
				}
				else if(next_state == STATE_GO_FORWARD) {
					forward();
				}
				else if(next_state == STATE_ROTATO_RIGHT) {
					right();
				}
				else if(next_state == STATE_STOP) {
					stop();
				}
			}
		}
	}
	
}

/////////////////////////////////////////////////////////////////////
// MODULE   : setup()
// FUNCTION : �����ݒ�
// RETURN   : �Ȃ�
/////////////////////////////////////////////////////////////////////
void setup()
{
	// Serial�ʐM�̏�����
	Serial.begin(9600);
	
	// I/O Pin�����ݒ�
	pinMode(IO_PIN_LINETRACK_LEFT, INPUT);
	pinMode(IO_PIN_LINETRACK_CENTER, INPUT);
	pinMode(IO_PIN_LINETRACK_RIGHT, INPUT);
	pinMode(IO_PIN_MOTOR_ENA, OUTPUT);
	pinMode(IO_PIN_MOTOR_ENB, OUTPUT);
	pinMode(IO_PIN_MOTOR_1, OUTPUT);
	pinMode(IO_PIN_MOTOR_2, OUTPUT);
	pinMode(IO_PIN_MOTOR_3, OUTPUT);
	pinMode(IO_PIN_MOTOR_4, OUTPUT);

	// DAC�ݒ�
	ledcSetup(0, 980, 8);
	ledcSetup(1, 980, 8);
	ledcAttachPin(IO_PIN_MOTOR_ENA, 0);
	ledcAttachPin(IO_PIN_MOTOR_ENB, 1);

	xTaskCreatePinnedToCore(Task_line_trace, "Task_line_trace", 2048, NULL, 1, NULL, 0);
	
	// �ŏ��͒��i
	forward();
}

/////////////////////////////////////////////////////////////////////
// MODULE   : loop()
// FUNCTION : ���C�����[�v
// RETURN   : �Ȃ�
/////////////////////////////////////////////////////////////////////
void loop() 
{
	// �����͑S��Task�ōs���̂Ń��C�����[�v���ł͉������Ȃ�
}

