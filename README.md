# HaptFlelt

> 🚧 **Projeto em desenvolvimento.** Protótipo em fase de aprimoramento. A versão atual do código-fonte será divulgada em breve.

## Sobre o projeto

HaptFlelt é um cinto de feedback háptico capaz de detectar movimentos do corpo (inclinações/movimentos para frente, trás, cima, baixo, esquerda e direita) e toques em áreas sensíveis, comunicando tudo via Bluetooth com um computador. Além disso, recebe de volta comandos que acionam motores de vibração no cinto.

O projeto tem duas partes:

- **`haptflelt/`** — firmware que roda no ESP32 embarcado no cinto (sensores, vibração, Bluetooth).
- **`haptflelt-bt/`** — daemon Python que roda no PC e faz a ponte de comunicação com o cinto.

## Funcionalidades

- Detecção de movimento em 6 direções (`UP`, `DOWN`, `FRONT`, `BACK`, `LEFT`, `RIGHT`) via MPU6050, com calibração automática da posição neutra.
- Quatro áreas de toque, isoladas ou combinadas.
- Comando secreto (toque simultâneo nas 4 áreas) para recalibrar a posição neutra a qualquer momento.
- Feedback vibratório em motores posicionados no cinto, acionado por comandos recebidos do PC.
- Indicadores visuais (LED RGB) e sonoros (buzzer) de status.
- Comunicação via Bluetooth Classic (SPP) — o cinto não precisa de cabo conectado ao PC, só de alimentação.
- Daemon Python que detecta a porta Bluetooth automaticamente e reconecta sozinho se a conexão cair.

## Hardware necessário

ESP32, MPU6050, 4 botões/áreas de toque, 4 motores de vibração, LED RGB, buzzer.

## Como rodar

**Firmware** (via [PlatformIO](https://platformio.org/)):
```bash
cd HaptFlelt
pio run --target upload
```

**Daemon do PC** (Python 3, cinto já pareado via Bluetooth):
```bash
cd haptflelt-bt
pip install -r requirements.txt
python main.py
```

## Status

Protótipo funcional.

## Licença

*A definir.*
