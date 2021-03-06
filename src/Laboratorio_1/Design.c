/////////////////////////////////////////////////////////////////////////
//  	Laboratorio #1: Luz de noche accionada por voz.
//	Estudiantes:
//				-Felipe Rincón Riveros (B25530)
//				-Fabián	Meléndez Bolaños (B24056)
//
/////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////
// Programa principal
/////////////////////////////////////////////////////////////////////////

// 1 - Llamado a subrutina de configuración del uC.
//
// void initial_setup()
//
// Se encarga de configurar los puertos de entrada salida, la latencia
// del módulo de tiempos, el micrófono (A/D),
//
// Aún no activa las interrupciones.

// 2 - Decidir el modo de las lámparas, configuracion de usuario
// Preguntar al profe para definir cómo hacer esto.

// 3 - Rutina inicial para confirmacion de configuración.
//
// void initial_blinking()
//
// Rutina de parpadeo de los LEDS, es bloqueante, hace que los LEDs par-
// padeen 3 veces.

// 4 - Deteccion del estado inicial
//
// bool check_initial_state()
//
// Esta función verifica el nivel de brillo y decide si iniciar encendi-
// do o apagado.

// 5 - Activar las interrupciones
//
// void activate_interruptions()
//
// Activa las interrupciones de:
// - Modulo de Tiempos
// - Conversior A/D -- Microfono (?)
// - Interrupción del botón manual

// 6 - End LOOP
//
// while(1) {}


/////////////////////////////////////////////////////////////////////////
// Subrutinas
/////////////////////////////////////////////////////////////////////////

// 1- Calcular promedio de datos
// 2- Determinar el estado según el nivel de luz


/////////////////////////////////////////////////////////////////////////
// Interrupciones
/////////////////////////////////////////////////////////////////////////

//	1 -	Interrupción por sonido: Se deben guardar los datos obtenidos 
//		en los primeros 5 seg después de la interrupción, calcular el
//		el promedio y determinar si es menor o mayor en un 10% del valor 
//		promedio de 1 segundo después. Si es mayor, se pone en alto una 
//		bandera para encerder la lámpara.
// 
// 	2 -	Interrupción por botón de control: Si la lámpara esta apagada,
//		levanta la bandera de encendido por 30 min.
//
//	3 -	Interrupción de módulo de tiempos: Cada cierto tiempo se revisa
//		la bandera de encendido de los LEDs. En caso de estar en alto, 
//		se encienden y se lleva un contador para determinar si han pasado
//		30 seg.
