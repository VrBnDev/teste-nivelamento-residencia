# 📢 Sistema de Alerta com OLED e Buzzer  

Este projeto tem como objetivo desenvolver um sistema embarcado que utilize um display OLED SSD1306 para exibir mensagens e um buzzer para emitir alertas sonoros. O sistema deve alternar entre estados de espera e atividade com um botão físico, simulando um alerta em ambientes industriais ou educacionais.

## 📌 Funcionalidades  
- ✅ Exibição de mensagens no display OLED SSD1306
- ✅ Emissão de alerta sonoro ao iniciar uma atividade
- ✅ Alternância entre estados de "Espera" e "Execução" ao pressionar um botão físico  

## 🛠️ Hardware Utilizado  
- **Microcontrolador:** Raspberry Pi Pico
- **Display:** OLED SSD1306 via I2C
- **Buzzer:** Ativado via PWM
- **Botão:** Para alternar os estados do sistema 

## 🎯 Como Funciona  
1. O sistema inicia no modo "Em Espera", exibindo a mensagem correspondente no display.  
2. Ao pressionar o botão, o sistema alterna para o modo "Atividade em Execução", acionando o buzzer.
3. O buzzer emite um bipe de 500ms sempre que a atividade estiver ativa.
4. Pressionando o botão novamente, o sistema retorna ao modo de espera.

