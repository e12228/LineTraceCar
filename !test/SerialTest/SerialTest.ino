
void setup() 
{
	// �]�����[�g��9600bps�ɐݒ�
	Serial.begin(9600);
}

void loop() 
{
	int i;
	int time;
	for(i = 0; i < 5; i ++) {
		// �o�ߎ��Ԏ擾
		time = millis();
		// �V���A�����j�^�֏o��
		Serial.print(time); // �V���A���֎��ԏo�́i���s�Ȃ��j
		Serial.print("[msec], ");
		// 1000msec�҂�
		delay(1000);
	}
	Serial.println(); // ���s
	Serial.println("Hello World"); // ������o�́i���s����j
	Serial.println(); // ���s

	delay(2000);
}
