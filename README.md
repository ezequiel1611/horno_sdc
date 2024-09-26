# HornoESP32 1.0

En el marco de una beca de servicio del Laboratorio de Control de la UTN Facultad Regional Paraná se realizó una actualización de un horno usado con fines académicos de forma que ahora el mismo use una placa de desarrollo ESP32 DevKit V1 como microcontrolador dotando así al equipo de la capacidad de conexión a internet. El microcontrolador maneja la potencia del horno mediante el uso de un TRIAC con control de disparo por ángulo de fase y cuenta con un sensor DHT11 para monitorear la temperatura del horno. Mediante la implementación de una base de datos en tiempo real (RTDB por Real Time Database) a través de la plataforma de Google Firebase es que interactúan tanto el microcontrolador como la aplicación web desarrollada para el control y monitoreo del sistema. El microcontrolador realiza peticiones a la RTDB cada 5 segundos actualizando la información de la temperatura del horno y verificando si la potencia empleada es la seleccionada por el usuario, mientras que al mismo tiempo la aplicación web lee cada segundo de la RTDB la información de la temperatura y la muestra en un gráfico temporal además de proveerle al usuario de un control deslizable para seleccionar la potencia del horno en una escala del 0% al 100%.

# Control de Potencia

El control de disparo del TRIAC por ángulo de fase se basa en la conexión de la tensión de red con la carga en cualquier punto de la semionda de forma sincronizada, de forma que se puede "recortar" la tensión sinusoidal en sectores más pequeños regulando así la potencia aplicada a la carga.

En la siguiente imagen se puede ver que al entregar el 50% de la potencia a la carga el forma de onda sinusoidal se ve "recortada" a la mitad de cada semiciclo.
![Forma de Onda con 50% de la Potencia](https://github.com/ezequiel1611/horno_sdc/blob/main/Images/tension_salida_50.jpeg)

El circuito empleado para lograr este funcionamiento es el siguiente:
![Circuito](https://github.com/ezequiel1611/horno_sdc/blob/main/Images/circuit_diagram.png)

Para más información sobre el diseño y teoría de funcionamiento del circuito, puede leer la documentación provista: [Teoría y Diseño Dimmer](https://github.com/ezequiel1611/horno_sdc/blob/main/Dimmer/TP2_Potencia.pdf)

# Introducción a Firebase y sus Beneficios

Firebase es una plataforma desarrollada por Google que ofrece una amplia gama de servicios para el desarrollo de aplicaciones móviles y web. Uno de los servicios más destacados que ofrece Firebase es su **base de datos en tiempo real**, que proporciona una solución eficiente para almacenar y sincronizar datos en tiempo real entre usuarios y dispositivos.

## Beneficios de usar la base de datos en tiempo real de Firebase:

1. **Sincronización en tiempo real:**  
   Los cambios realizados en la base de datos se reflejan instantáneamente en todos los dispositivos conectados. Esto facilita la creación de aplicaciones colaborativas y en tiempo real donde los usuarios pueden ver actualizaciones al instante.

2. **Facilidad de integración:**  
   Firebase ofrece bibliotecas para una variedad de plataformas y frameworks, lo que simplifica la integración en aplicaciones móviles y web existentes.

3. **Seguridad y autenticación:**  
   Firebase proporciona reglas de seguridad personalizables y autenticación integrada con otros servicios de Google, lo que ayuda a proteger los datos del usuario y controlar el acceso a la base de datos.

4. **Escalabilidad y disponibilidad:**  
   Al ser una plataforma en la nube administrada por Google, Firebase ofrece una alta escalabilidad y disponibilidad, sin preocuparse por la gestión de la infraestructura subyacente.

Firebase provee una facilidad a la hora de programar el microcontrolador ESP32 ya que con conectar el mismo a internet ya es posible interactuar con la base de datos de forma rápida e intuitiva debido a las librerías provistas para su uso. Por otro lado, también permite el alojamiento de una aplicación web permitiendo así el uso de un dominio en internet para crear una página web que sirva para control y monitoreo del sistema.

# Aplicación Web

Usando la herramienta de desarrollo online Replit se diseño la página web para monitorear y controlar el HornoESP32. Este desarrollo fue llevado a cabo en HTML y sus archivos HTML, CSS y JS puede accederse a través de: [Replit-HornoESP32](https://replit.com/@lautaroezequie3/HornoSDC)

## Monitoreo de la Temperatura

Al ingresar a la página usted verá un gráfico vacío, cuando haga click en el boton "On/Off" se habilitará la toma de datos y verá como cada segundo se imprime en la gráfica la temperatura del horno. Usted puede observar en tiempo real como varía la temperatura del horno mediante esta gráfica donde se indica en el eje X hora, minutos y segundos del dato tomado y en el eje Y la temperatura del horno en grados Celsius. Si quiere saber el valor exacto de un dato solo debe posar el mouse sobre el punto de la gráfica deseado, usted puede pausar la actualización de la gráfica haciendo click nuevamente en el botón "On/Off" si así lo desea.

Además de la visualización en tiempo real, la página web tiene un botón "Datos" el cual al presionarlo realiza la descarga de un archivo "datos.m" que contiene en cada renglón el dato de temperatura de cada punto de la gráfica. Esto con la finalidad de poder realizar un posterior análisis empleando GNU Octave o MatLab como se ve a continuación.

![test_octave](https://github.com/ezequiel1611/horno_sdc/blob/main/Images/plot_data_1.jpg)

## Control de la Temperatura

Una vez en la página web usted podrá ver controlador deslizable que indica la potencia del horno en una escala del 0% al 100%, este controlador tiene un paso de 10% por lo que usted podrá determinar la potencia del horno a voluntad mediante la variación de este controlador deslizable. Este control deslizable envia el dato de a que ángulo se debe realizar el disparo del TRIAC a la base de datos y es el ESP32 el cual luego lee esta información y ajusta el ángulo de disparo al deseado para aśi establecer la potencia del horno a la requerida por el usuario.

**[Aplicación Web](https://horno-sdc.web.app/)**

![preview app web](https://github.com/ezequiel1611/horno_sdc/blob/main/Images/test1.png)
