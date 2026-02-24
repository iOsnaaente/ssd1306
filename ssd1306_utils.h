/**
 * @file    ssd1306_utils.h
 * @brief   Utilitários para o controlador SSD1306
 * @details Este arquivo define funções utilitárias para o controlador SSD1306,
 *          como desenho de pontos e limpeza de buffer interno.
 * @note    Este arquivo deve ser incluído apenas dentro do código do driver SSD1306.
 * 
 * @author  Bruno Gabriel Flores Sampaio
 * @date    Criado em 24 de fevereiro de 2026
 */

#pragma once 

#include "ssd1306.h"
#include <cmath>
#include <cstring>

/**
 * @brief   Acende um pixel apenas se estiver dentro dos limites do display
 * @details Essa função é uma camada de segurança para evitar que seja 
 *          tentado acender pixels fora dos limites do display, o que poderia
 *          causar problemas de hardware ou comportamento inesperado.
 * @param   dev Ponteiro para a estrutura do dispositivo OLED.
 * @param   x Coordenada x do pixel a ser aceso.
 * @param   y Coordenada y do pixel a ser aceso.
 * @return  True se o pixel foi aceso com sucesso
 * @return  False se estava fora dos limites.
 */
inline bool ssd1306_pixel_safe( SSD1306_t* dev, int x, int y );


/**
 * @brief   Desenha um ponto com um tamanho específico 
 *          (0 = 1 pixel, 1 = 3x3, 2 = 5x5)
 * @details O ponto é desenhado como um quadrado centrado na posição 
 *          (x, y). O tamanho determina a extensão do quadrado:
 * @note    O tamanho é limitado a 0, 1 ou 2 para evitar que o ponto 
 *          fique muito grande e ultrapasse os limites do display. 
 *          O desenho é feito chamando a função ssd1306_pixel_safe para 
 *          garantir que os pixels sejam acesos apenas dentro dos 
 *          limites do display.
 * @param   dev Ponteiro para a estrutura do dispositivo OLED.
 * @param   x Coordenada x do centro do ponto.
 * @param   y Coordenada y do centro do ponto.
 * @param   size Tamanho do ponto (0, 1 ou 2).
 * @return  void
 */
inline void ssd1306_draw_dot_default( 
    SSD1306_t* dev, 
    uint8_t x, 
    uint8_t y, 
    uint8_t size,
    bool    fill = false
);


/**
 * @brief   Desenha um círculo usando o algoritmo de Bresenham
 * @details O algoritmo de Bresenham é um método eficiente para desenhar
 *          círculos em uma grade de pixels. Ele calcula os pontos do círculo
 *          usando apenas operações inteiras. 
 * @note    O círculo é desenhado centrado em (xc, yc) com um raio de r.
 * @param   dev Ponteiro para a estrutura do dispositivo OLED.
 * @param   xc Coordenada x do centro do círculo.
 * @param   yc Coordenada y do centro do círculo.
 * @param   r Raio do círculo.
 * @return  void
 * 
 */
inline void ssd1306_draw_dot(
    SSD1306_t *dev,
    int xc, int yc,
    int size, 
    bool fill = false
);


/**
 * @brief   Limpa o buffer interno do dispositivo OLED
 * @details O buffer interno é organizado em páginas, onde cada página 
 *          representa uma linha de 8 pixels. A função percorre cada 
 *          página e preenche os segmentos com o valor 0x00, apagando 
 *          assim todos os pixels do display.
 * @param   dev Ponteiro para a estrutura do dispositivo OLED.
 * @return  void
 */
void ssd1306_clear_internal_buffer(SSD1306_t *dev);


/**
 * @brief   Renderiza um spinner de carregamento no display OLED
 * @details O spinner é composto por pontos que giram em torno do centro 
 *          do display. A função calcula a posição de cada ponto com base 
 *          em um ângulo que é incrementado a cada chamada, criando a 
 *          ilusão de movimento. O parâmetro mirror inverte a direção do 
 *          giro para criar uma animação mais dinâmica.
 * @param   dev Ponteiro para a estrutura do dispositivo OLED.
 * @param   mirror Se true, o spinner gira no sentido anti-horário; 
 *          se false, gira no sentido horário.
 * @return  void
 */
void ssd1306_render_loading_spinner( 
    SSD1306_t* dev, 
    bool mirror,
    int SPINNER_POINTS = 8,
    int SPINNER_RADIUS = 20,
    double SPINNER_SPEED = 0.1
);