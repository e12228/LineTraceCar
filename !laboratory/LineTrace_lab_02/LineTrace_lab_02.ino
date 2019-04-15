////////////////////////////////////////////////////////////////////////////////
//
//		LineTrace_lab_02.ino
//			
//			LineTrace�����Task��œ��삳����
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
	Serial.println("go forward!");
}

/////////////////////////////////////////////////////////////////////
// MODULE   : back()
// FUNCTION : ���
// RETURN   : �Ȃ�
/////////////////////////////////////////////////////////////////////
void back()
{
	ledcWrite(0, MOTOR_SPEED);  
	ledcWrite(1, MOTOR_SPEED);  
	digitalWrite(IO_PIN_MOTOR_1, LOW);
	digitalWrite(IO_PIN_MOTOR_2, HIGH);
	digitalWrite(IO_PIN_MOTOR_3, HIGH);
	digitalWrite(IO_PIN_MOTOR_4, LOW);
	Serial.println("go back!");
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
	Serial.println("go right!");
} 

/////////////////////////////////////////////////////////////////////
// MODULE   : stop()
// FUNCTION : ��~
// RETURN   : �Ȃ�
/////////////////////////////////////////////////////////////////////
void stop()
{
	digitalWrite(IO_PIN_MOTOR_ENA, LOW);
	digitalWrite(IO_PIN_MOTOR_ENB, LOW);
	Serial.println("Stop!");
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

		if(read_sensor_C()){
			forward();
		}
		else if(read_sensor_R()) { 
			right();
			while(read_sensor_R());                             
		}   
		else if(read_sensor_L()) {
			left();
			while(read_sensor_L());  
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

