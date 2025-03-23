#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/clocks.h"
#include "hardware/gpio.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "pico/binary_info.h"
#include "inc/ssd1306.h"
#include "hardware/i2c.h"

// Configuração da pinagem 
#define BUZZER_PIN 21 
#define BUTTON_PIN 5
const uint I2C_SDA = 14;
const uint I2C_SLC = 15;

// Variável de controle de pausa
volatile bool paused = false; 

void toggle_pause(uint gpio, uint32_t events){
    paused = !paused;
}

// Configuração da frequência do buzzer (em Hz)
#define BUZZER_FREQUENCY 100

// Definição de uma função para inicializar o PWM no pino do buzzer
void pwm_init_buzzer(uint pin) {
    // Configurar o pino como saída de PWM
    gpio_set_function(pin, GPIO_FUNC_PWM);

    // Obter o slice do PWM associado ao pino
    uint slice_num = pwm_gpio_to_slice_num(pin);

    // Configurar o PWM com frequência desejada
    pwm_config config = pwm_get_default_config();
    pwm_config_set_clkdiv(&config, clock_get_hz(clk_sys) / (BUZZER_FREQUENCY * 4096)); // Divisor de clock
    pwm_init(slice_num, &config, true);

    // Iniciar o PWM no nível baixo
    pwm_set_gpio_level(pin, 0);
}

// Definição de uma função para emitir um alarme sonoro durante duração de atividade
void beep(uint pin, uint duration_ms) {
    // Obter o slice do PWM associado ao pino
    uint slice_num = pwm_gpio_to_slice_num(pin);

    // Configurar o duty cycle para 50% (ativo)
    pwm_set_gpio_level(pin, 2048);

    // Temporização
    sleep_ms(duration_ms);

    // Desativar o sinal PWM (duty cycle 0)
    pwm_set_gpio_level(pin, 0);

    // Pausa entre os beeps
    sleep_ms(100); // Pausa de 100ms
}

int main() {
    // Inicializar o sistema de saída padrão
    stdio_init_all();

    // Inicializar o PWM no pino do buzzer
    pwm_init_buzzer(BUZZER_PIN);

    // Inicializar o botão 
    gpio_init(BUTTON_PIN);
    gpio_set_dir(BUTTON_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_PIN);
    gpio_set_irq_enabled_with_callback(BUTTON_PIN, GPIO_IRQ_EDGE_FALL, true, &toggle_pause);

    // Inicialização do I2C 
    i2c_init(i2c1, ssd1306_i2c_clock * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SLC, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SLC);

    // Inicialização do OLED SSD1306
    ssd1306_init();

    struct render_area frame_area = {
        start_column : 0,
        end_column : ssd1306_width - 1,
        start_page : 0,
        end_page : ssd1306_n_pages - 1
    };

    calculate_render_area_buffer_length(&frame_area);

    uint8_t ssd[ssd1306_buffer_length];
    memset(ssd, 0, ssd1306_buffer_length);
    render_on_display(ssd, &frame_area);

    char *estadoespera[] = {
        "   Sistema  ",
        "  Em Espera "
    };

    char *atividade[] = {
        "  Atividade",
        "Em execucao!"
    };

    
    // Loop infinito
    while (true) {
        if (!paused){
            int y = 0;
            for (uint i = 0; i < count_of(estadoespera); i++)
            {
                ssd1306_draw_string(ssd, 5, y, estadoespera[i]);
                y += 8;
            }
            render_on_display(ssd, &frame_area);
            sleep_ms(1000);
        } else {
            beep(BUZZER_PIN, 500); // Bipe de 500ms
            int y = 0;
            for (uint i = 0; i < count_of(atividade); i++)
            {
                ssd1306_draw_string(ssd, 5, y, atividade[i]);
                y += 8;
            }
            render_on_display(ssd, &frame_area);
            sleep_ms(1000);
        }
    }
    return 0;
}
