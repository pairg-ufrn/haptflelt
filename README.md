# Cinto Háptico Flexível (HaptFlelt)

> **Protótipo funcional, em fase de aprimoramento.**

## Sobre o projeto

O HaptFlelt é um cinto de feedback háptico capaz de detectar movimentos do corpo (inclinações/movimentos para frente, trás, cima, baixo, esquerda e direita) e toques em áreas sensíveis, comunicando tudo via Bluetooth com um computador. Além disso, recebe de volta comandos que acionam motores de vibração no cinto.

O projeto tem duas partes:

- **`haptflelt/`**: firmware que roda no ESP32 embarcado no cinto (sensores, vibração, Bluetooth).
- **`haptflelt-bt/`**: aplicação Python que roda no PC e faz a ponte de comunicação com o cinto (daemon de terminal e aplicação desktop).

## Funcionalidades

- Detecção de movimento em 6 direções (`UP`, `DOWN`, `FRONT`, `BACK`, `LEFT`, `RIGHT`) via MPU6050, com calibração automática da posição neutra.
- Quatro áreas de toque, isoladas ou combinadas.
- Comando secreto (toque simultâneo nas 4 áreas) para recalibrar a posição neutra a qualquer momento.
- Feedback vibratório em motores posicionados no cinto, acionado por comandos recebidos do PC.
- Indicadores visuais (LED RGB) e sonoros (buzzer) de status.
- Comunicação via Bluetooth Classic (SPP) — o cinto não precisa de cabo conectado ao PC, só de alimentação.
- Aplicação Python que detecta a porta Bluetooth automaticamente e reconecta sozinha se a conexão cair.
- Aplicação desktop para configurar quais teclas do PC disparam cada vibração e quais teclas são simuladas a partir dos toques e movimentos do cinto.

## Hardware necessário

ESP32, MPU6050, 4 áreas de toque capacitivo, 4 motores de vibração, LED RGB, buzzer.

## Como rodar

**Firmware** (via [PlatformIO](https://platformio.org/)):
```bash
cd haptflelt
pio run --target upload
```

**PC** (Python 3, cinto já pareado via Bluetooth):
```bash
cd haptflelt-bt
pip install -r requirements.txt
python gui_app.py   # aplicação desktop
```
