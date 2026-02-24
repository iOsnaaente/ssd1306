/**
 * @file    ssd1306_utils.cpp
 * @brief   Utilitários para o controlador SSD1306
 * @details Este arquivo define funções utilitárias para o controlador SSD1306,
 *          como desenho de pontos e limpeza de buffer interno.
 * @note    Este arquivo deve ser incluído apenas dentro do código do driver SSD1306.
 * 
 * @author  Bruno Gabriel Flores Sampaio
 * @date    Criado em 24 de fevereiro de 2026
 */

#include "ssd1306_utils.h"

inline bool ssd1306_pixel_safe( SSD1306_t* dev, int x, int y ) {
    if ( y < 0 || y >= ssd1306_get_height(dev) ) return false;
    if ( x < 0 || x >= ssd1306_get_width(dev)  ) return false;
    _ssd1306_pixel( dev, x, y, false );
    return true;
}


inline void ssd1306_draw_dot_default( 
    SSD1306_t* dev, 
    uint8_t x, 
    uint8_t y, 
    uint8_t size,
    bool    fill
) {
    int32_t radius = size;
    for (int32_t dy = -radius; dy <= radius; dy++ ) {
        for (int32_t dx = -radius; dx <= radius; dx++) {
            int32_t d = dx*dx + dy*dy;
            if ( fill ) {
                if ( d <= radius*radius ) {
                    ssd1306_pixel_safe(dev, x + dx, y + dy);
                }
            } else {
                if ( abs(d - radius*radius) <= radius ) {
                    ssd1306_pixel_safe(dev, x + dx, y + dy);
                }
            }
        }
    }
}


inline void ssd1306_draw_dot(
    SSD1306_t *dev,
    int xc, int yc,
    int size, bool fill
){
    int r = size;
    if ( r == 0 ) {
        ssd1306_pixel_safe(dev, xc, yc);
        return;
    }
    int x = 0;
    int y = size;
    int d = 3 - 2 * size;

    while (y >= x) {
        // Fill = Preenche fatias horizontais
        if (fill) {
            for (int i = xc - x; i <= xc + x; i++) {
                ssd1306_pixel_safe(dev, i, yc + y);
                ssd1306_pixel_safe(dev, i, yc - y);
            }
            for (int i = xc - y; i <= xc + y; i++) {
                ssd1306_pixel_safe(dev, i, yc + x);
                ssd1306_pixel_safe(dev, i, yc - x);
            }
        } 
        // Not Fill = Apenas contorno (8 fatias simétricas)
        else {
            ssd1306_pixel_safe(dev, xc + x, yc + y);
            ssd1306_pixel_safe(dev, xc - x, yc + y);
            ssd1306_pixel_safe(dev, xc + x, yc - y);
            ssd1306_pixel_safe(dev, xc - x, yc - y);

            ssd1306_pixel_safe(dev, xc + y, yc + x);
            ssd1306_pixel_safe(dev, xc - y, yc + x);
            ssd1306_pixel_safe(dev, xc + y, yc - x);
            ssd1306_pixel_safe(dev, xc - y, yc - x);
        }
        if (d < 0) {
            d = d + 4 * x + 6;
        } else {
            d = d + 4 * (x - y) + 10;
            y--;
        }
        x++;
    }
}


void ssd1306_clear_internal_buffer(SSD1306_t *dev) {
    const int pages = ssd1306_get_pages(dev);
    for (int page = 0; page < pages; page++) {
        memset(
            dev->_page[page]._segs, 0x00,
            sizeof(dev->_page[page]._segs)
        );
    }
}


void ssd1306_render_loading_spinner( 
    SSD1306_t* dev, 
    bool mirror,
    int SPINNER_POINTS,
    int SPINNER_RADIUS,
    double SPINNER_SPEED
) {
    static float angle = 0.0f;
    // Pega o centro da tela 
    const int center_x = ssd1306_get_width(dev) / 2;
    const int center_y = ssd1306_get_height(dev) / 2;
    
    // Limpra o buffer 
    ssd1306_clear_internal_buffer(dev);
    
    for (int i = 0; i < SPINNER_POINTS; i++) {
        float point_angle = angle + (2.0f * M_PI * i / SPINNER_POINTS);
        if (mirror) {
            point_angle = -point_angle;
        }
        int x = center_x + (int32_t)(
            roundf(SPINNER_RADIUS * cosf(point_angle))
        );
        int y = center_y + (int32_t)(
            roundf(SPINNER_RADIUS * sinf(point_angle))
        );
        // Intensidade simulada (rastro)
        int brightness_index =
            (i + (int32_t)(angle * 10)) % SPINNER_POINTS;
        if (brightness_index == 0) {
            ssd1306_draw_dot(dev, x, y, 2, true);
        } else if (brightness_index == 1) {
            ssd1306_draw_dot(dev, x, y, 1, true);
        } else if (brightness_index <= SPINNER_POINTS/2 ) {
            ssd1306_draw_dot(dev, x, y, 0, false);
        }
    }

    angle += SPINNER_SPEED;
    if (angle > 2.0f * M_PI) {
        angle -= 2.0f * M_PI;
    }
    ssd1306_show_buffer(dev);
}
