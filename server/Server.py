import socket
import threading
import time

class RelayServer:
    """
    Servidor de retransmisión que maneja la comunicación entre sensores y actuadores.
    Recibe datos de distancia de los sensores y los reenvía a todos los actuadores conectados.
    """
    def __init__(self, host='192.168.0.60', sensor_port=8888, actuator_port=8889):
        """
        Inicializa el servidor con la configuración de red especificada.
        
        Args:
            host (str): Dirección IP en la que escuchar
            sensor_port (int): Puerto para escuchar conexiones de sensores
            actuator_port (int): Puerto para escuchar conexiones de actuadores
        """
        self.host = host
        self.sensor_port = sensor_port
        self.actuator_port = actuator_port
        
        # Estado del sistema
        self.current_distance = 0
        self.distance_lock = threading.Lock()
        self.actuator_clients = []
        self.actuator_clients_lock = threading.Lock()
        
        # Sockets del servidor
        self.sensor_server = None
        self.actuator_server = None
        
        # Flag para control de ejecución
        self.running = False
    
    def start(self):
        """Inicia los servidores de sensores y actuadores en hilos separados."""
        # Inicializar y configurar el servidor para sensores
        self.sensor_server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.sensor_server.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        self.sensor_server.bind((self.host, self.sensor_port))
        self.sensor_server.listen(5)
        print(f"Servidor para sensores iniciado en {self.host}:{self.sensor_port}")
        
        # Inicializar y configurar el servidor para actuadores
        self.actuator_server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.actuator_server.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        self.actuator_server.bind((self.host, self.actuator_port))
        self.actuator_server.listen(5)
        print(f"Servidor para actuadores iniciado en {self.host}:{self.actuator_port}")
        
        # Establecer flag de ejecución
        self.running = True
        
        # Iniciar hilos para aceptar conexiones
        threading.Thread(target=self._accept_sensor_connections, daemon=True).start()
        threading.Thread(target=self._accept_actuator_connections, daemon=True).start()
        
        # Mostrar información de funcionamiento
        print("==== Servidor de retransmisión ====")
        print(f"El servidor recibe distancias del sensor en el puerto {self.sensor_port}")
        print(f"El servidor reenvía las distancias a los actuadores en el puerto {self.actuator_port}")
        print("La lógica de control de LEDs ahora está en el actuador")
        print("===================================")
        
        # Mantener el programa principal ejecutándose hasta que se solicite detener
        try:
            while self.running:
                time.sleep(1)  # Dormir para no consumir CPU innecesariamente
        except KeyboardInterrupt:
            self.stop()
    
    def stop(self):
        """Detiene el servidor y cierra todas las conexiones."""
        print("Cerrando el servidor...")
        self.running = False
        
        # Cerrar servidores
        if self.sensor_server:
            self.sensor_server.close()
        if self.actuator_server:
            self.actuator_server.close()
        
        # Cerrar conexiones con actuadores
        with self.actuator_clients_lock:
            for client in self.actuator_clients:
                try:
                    client.close()
                except:
                    pass
            self.actuator_clients.clear()
    
    def _accept_sensor_connections(self):
        """Acepta continuamente nuevas conexiones de sensores y crea un hilo para cada una."""
        while self.running:
            try:
                client_socket, client_address = self.sensor_server.accept()
                # Crear un hilo dedicado para manejar esta conexión
                threading.Thread(
                    target=self._handle_sensor, 
                    args=(client_socket, client_address)
                ).start()
            except Exception as e:
                if self.running:  # Solo mostrar errores si todavía estamos en ejecución
                    print(f"Error aceptando conexión de sensor: {e}")
    
    def _accept_actuator_connections(self):
        """Acepta continuamente nuevas conexiones de actuadores y crea un hilo para cada una."""
        while self.running:
            try:
                client_socket, client_address = self.actuator_server.accept()
                # Crear un hilo dedicado para manejar esta conexión
                threading.Thread(
                    target=self._handle_actuator, 
                    args=(client_socket, client_address)
                ).start()
            except Exception as e:
                if self.running:  # Solo mostrar errores si todavía estamos en ejecución
                    print(f"Error aceptando conexión de actuador: {e}")
    
    def _handle_sensor(self, client_socket, client_address):
        """
        Maneja las conexiones de los sensores ESP32.
        Recibe datos de distancia, los procesa y los reenvía a todos los actuadores.
        
        Args:
            client_socket: Socket de la conexión con el sensor
            client_address: Dirección IP y puerto del sensor conectado
        """
        print(f"Sensor conectado desde {client_address}")
        
        buffer = ""  # Para acumular datos parciales que pueden llegar fragmentados
        
        try:
            while self.running:
                # Recibir datos del sensor (bloqueante hasta que lleguen datos)
                data = client_socket.recv(1024).decode('utf-8')
                if not data:
                    # Si no hay datos, significa que el sensor se desconectó
                    break
                
                buffer += data  # Acumular en el buffer para procesar líneas completas
                
                # Procesar todas las líneas completas disponibles en el buffer
                while '\n' in buffer:
                    line, buffer = buffer.split('\n', 1)  # Separa una línea completa
                    line = line.strip()  # Eliminar espacios en blanco
                    
                    if not line:
                        continue  # Ignorar líneas vacías
                        
                    try:
                        # Convertir el texto recibido a un valor numérico
                        distance = float(line)
                        
                        # Actualizar la distancia actual protegiendo con el lock
                        with self.distance_lock:
                            self.current_distance = distance
                        
                        print(f"Distancia recibida: {distance} cm")
                        
                        # Transmitir la distancia a todos los actuadores conectados
                        self._broadcast_distance(distance)
                        
                    except ValueError:
                        # Si los datos no son un número válido
                        print(f"Datos inválidos recibidos: {line}")
        except Exception as e:
            # Capturar cualquier error en la comunicación
            if self.running:  # Solo mostrar errores si todavía estamos en ejecución
                print(f"Error en la conexión del sensor: {e}")
        finally:
            # Asegurar que el socket se cierre correctamente al terminar
            client_socket.close()
            print(f"Sensor desconectado: {client_address}")
    
    def _handle_actuator(self, client_socket, client_address):
        """
        Maneja las conexiones de los actuadores ESP32.
        Envía la distancia actual al conectarse y mantiene abierta la conexión.
        
        Args:
            client_socket: Socket de la conexión con el actuador
            client_address: Dirección IP y puerto del actuador conectado
        """
        print(f"Actuador conectado desde {client_address}")
        
        # Registrar el cliente en la lista de actuadores
        with self.actuator_clients_lock:
            self.actuator_clients.append(client_socket)
        
        try:
            # Obtener la distancia actual para enviarla inmediatamente al nuevo actuador
            with self.distance_lock:
                distance = self.current_distance
            
            # Enviar distancia inicial al actuador recién conectado
            command = f"DIST:{distance}\n"
            client_socket.send(command.encode('utf-8'))
            
            # Mantener la conexión abierta y verificar periódicamente si sigue activa
            while self.running:
                # Esperar un tiempo antes de verificar la conexión
                time.sleep(1.0)
                
                # Comprobar si la conexión sigue activa
                try:
                    # Envío de 0 bytes como "heartbeat" para verificar la conexión
                    client_socket.send(b'')
                except:
                    print(f"Actuador {client_address} desconectado")
                    break
                    
        except Exception as e:
            # Capturar cualquier error en la comunicación
            if self.running:  # Solo mostrar errores si todavía estamos en ejecución
                print(f"Error en la conexión del actuador: {e}")
        finally:
            # Eliminar el cliente de la lista de actuadores y cerrar el socket
            with self.actuator_clients_lock:
                if client_socket in self.actuator_clients:
                    self.actuator_clients.remove(client_socket)
            client_socket.close()
            print(f"Actuador desconectado: {client_address}")
    
    def _broadcast_distance(self, distance):
        """
        Envía la distancia actual a todos los actuadores conectados.
        
        Args:
            distance: Valor de distancia (float) a enviar a los actuadores
        """
        with self.actuator_clients_lock:
            # Crear una copia de la lista para evitar modificarla durante la iteración
            clients_to_update = self.actuator_clients.copy()
        
        # Preparar el comando en formato texto con salto de línea como terminador
        command = f"DIST:{distance}\n"
        print(f"Enviando distancia {distance}cm a {len(clients_to_update)} actuadores")
        
        # Enviar a cada actuador conectado
        for client in clients_to_update:
            try:
                client.send(command.encode('utf-8'))
            except Exception as e:
                print(f"Error al enviar distancia a un actuador: {e}")
                # No eliminamos el cliente aquí para evitar modificar la lista
                # durante la iteración. Los clientes inválidos se eliminarán
                # en sus respectivos hilos actuator_handler

# Punto de entrada del programa
if __name__ == "__main__":
    server = RelayServer()
    server.start()