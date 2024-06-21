#include <Servo.h>

double ANGULO_CENTRO;
double dist_ade;
double dist_izq_x;
double dis_y_final;
double VEL;
double dis_izq;
double pausa;
double dist_der_x;
double dist_ade_y;
double vel_cur;
double vel_ap;
double dis_der;
double angulo_centro_real;
double angulo_izq;
double dist23;
double dist_23_inicio;
double angulo_der;
double dist_ade_delay;
double dist_inicio_der;
double c_cruzes;
double dist_ade_inicio;
double var_ang;
double dist_inicio_izq;
Servo servo_A5;
Servo servo_12;
unsigned long task_time_ms=0;

void datos_x() {
	dist_izq_x = dis_izq;
	dist_der_x = dis_der;
}
void datos_y() {
	dis_y_final = 175;
	dist_ade_y = dist_ade;
}
double fnc_ultrasonic_distance(int _t, int _e){
	unsigned long dur=0;
	digitalWrite(_t, LOW);
	delayMicroseconds(5);
	digitalWrite(_t, HIGH);
	delayMicroseconds(10);
	digitalWrite(_t, LOW);
	dur = pulseIn(_e, HIGH, 18000);
	if(dur==0)return 999.0;
	return (dur/57);
}

void valores_iniciales() {
	VEL = 150;
	vel_cur = 150;
	angulo_centro_real = 81;
	ANGULO_CENTRO = 81;
	servo_A5.write(90);
	servo_12.write(ANGULO_CENTRO);
	var_ang = 6;
	dist_inicio_izq = fnc_ultrasonic_distance(3,2);
	dist_inicio_der = fnc_ultrasonic_distance(9,10);
	dist_ade_inicio = fnc_ultrasonic_distance(6,4);
	dist_23_inicio = (20 + ((dist_inicio_der + dist_inicio_izq)));
	pausa=pausa+(1);
}
void sensores() {
	dis_izq = fnc_ultrasonic_distance(3,2);
	dist_ade = fnc_ultrasonic_distance(6,4);
	dis_der = fnc_ultrasonic_distance(9,10);
	dist23 = (dis_izq + dis_der);
	if((millis()-task_time_ms)>=2000){
		task_time_ms=millis();
		dist_ade_delay = fnc_ultrasonic_distance(6,4);
	}
}
void valores_ccero() {
	pausa = 0;
	vel_ap = 0;
	dist_ade = 0;
	dis_izq = 0;
	dist_izq_x = 0;
	angulo_izq = 0;
	angulo_der = 0;
	dist_ade_y = 0;
	dist_der_x = 0;
}
void calculo() {
	datos_y();
	datos_x();
	angulo_izq = ((atan(((((dist_ade_y + 10.5)) / dist_izq_x))) * ((180 / 3.14))) - var_ang);
	angulo_der = (((180 - (atan(((((((dist_ade_y + 10.5)) / 2)) / dist_der_x))) * ((180 / 3.14))))) - var_ang);
}
void giro_izquierdad() {
	digitalWrite(7,LOW);
	digitalWrite(8,HIGH);
	analogWrite(5,vel_cur);
	servo_12.write(angulo_izq);
}
void adelante() {
	if ((dist23 < dist_23_inicio)) {
		while ((dist23 < dist_23_inicio)) {
			servo_12.write(ANGULO_CENTRO);
			digitalWrite(7,LOW);
			digitalWrite(8,HIGH);
			analogWrite(5,VEL);
			sensores();
			corregir();
		}
	}

}
void giro_derecho() {
	digitalWrite(7,LOW);
	digitalWrite(8,HIGH);
	analogWrite(5,vel_cur);
	servo_12.write(angulo_der);
}
void corregir() {
	if ((dist23 < dist_23_inicio)) {
		if ((dist_inicio_der > dis_der)) {
			ANGULO_CENTRO = angulo_centro_real;
			ANGULO_CENTRO = (ANGULO_CENTRO - 3);
		}

		if ((dist_inicio_izq > dis_izq)) {
			ANGULO_CENTRO = angulo_centro_real;
			ANGULO_CENTRO=ANGULO_CENTRO+(3);
		}

	}

}
void completado() {
	if (((c_cruzes == 12) && (dist_ade_inicio >= dist_ade))) {
		while(true);
	}

}
void retrodecer() {
	digitalWrite(7,HIGH);
	digitalWrite(8,LOW);
	analogWrite(5,VEL);
}
void apagar() {
	digitalWrite(7,LOW);
	digitalWrite(8,LOW);
	analogWrite(5,vel_ap);
}
void choque() {
	if ((dist_ade == dist_ade_delay)) {
		while (!((dist_ade >= 50))) {
			sensores();
			retrodecer();
		}
	}

}
void curva_inzquierda() {
	if (((dist23 > dist_23_inicio) && (dis_izq > dis_der))) {
		calculo();
		while (!(((dist_ade > dis_y_final) && (300 < dist_ade)))) {
			sensores();
			giro_izquierdad();
		}
		c_cruzes=c_cruzes+(1);
	}

}
void curva_derecha() {
	if (((dist23 > dist_23_inicio) && (dis_der > dis_izq))) {
		calculo();
		while (!(((dist_ade > dis_y_final) && (300 < dist_ade)))) {
			giro_derecho();
			sensores();
		}
		c_cruzes=c_cruzes+(1);
	}

}

void setup()
{
  	servo_A5.attach(A5);
	servo_12.attach(12);
	pinMode(3, OUTPUT);
	pinMode(2, INPUT);
	pinMode(9, OUTPUT);
	pinMode(10, INPUT);
	pinMode(6, OUTPUT);
	pinMode(4, INPUT);
	pinMode(7, OUTPUT);
	pinMode(8, OUTPUT);
	pinMode(5, OUTPUT);

	delay(2500);
	valores_ccero();
	valores_iniciales();

}


void loop()
{

  	sensores();
  	calculo();
  	adelante();
  	curva_derecha();
  	curva_inzquierda();
  	choque();
  	completado();

}