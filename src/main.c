#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>

#define TIME_VERMELHO_MS 4000 // Vermelho
#define TIME_VERDE_MS 2000    // Verde 
#define TIME_AMARELO_MS 1000   // Amarelo

// Define o LED usando Device Tree
#define LEDRED_NODE DT_ALIAS(led2)
#define LEDGREEN_NODE DT_ALIAS(led0)

// Verifica se o LED está definido no Device Tree
#if DT_NODE_HAS_STATUS(LEDRED_NODE, okay)
static const struct gpio_dt_spec ledred = GPIO_DT_SPEC_GET(LEDRED_NODE, gpios);
#else
#error "Unsupported board: led2 devicetree alias is not defined"
#endif

#if DT_NODE_HAS_STATUS(LEDGREEN_NODE, okay)
static const struct gpio_dt_spec ledgreen = GPIO_DT_SPEC_GET(LEDGREEN_NODE, gpios);
#else
#error "Unsupported board: led0 devicetree alias is not defined"
#endif

void main(void)
{
    int retred;
	int retgreen;

    // Verifica se o device está pronto
    if (!gpio_is_ready_dt(&ledred)) {
        printk("Error: LED RED device %s is not ready\n", ledred.port->name);
        return;
    }

	if (!gpio_is_ready_dt(&ledgreen)) {
        printk("Error: LED GREEN device %s is not ready\n", ledgreen.port->name);
        return;
    }


    // Configura o pino como saída
    retred = gpio_pin_configure_dt(&ledred, GPIO_OUTPUT_ACTIVE);
    if (retred < 0) {
        printk("Error %d: failed to configure LED RED pin\n", retred);
        return;
    }
	retgreen = gpio_pin_configure_dt(&ledgreen, GPIO_OUTPUT_ACTIVE);
	if (retgreen < 0) {
        printk("Error %d: failed to configure LED GREEN pin\n", retgreen);
        return;
    }

    while (1) {

		// Estado Vermelho
        gpio_pin_set_dt(&ledred, 1);
		gpio_pin_set_dt(&ledgreen, 0);

        k_msleep(TIME_VERMELHO_MS);

		// Estado Verde
		gpio_pin_set_dt(&ledred, 0);
		gpio_pin_set_dt(&ledgreen, 1);

		k_msleep(TIME_VERDE_MS);

		// Estado Amarelo
		gpio_pin_set_dt(&ledred, 1);
		gpio_pin_set_dt(&ledgreen, 1);

		k_msleep(TIME_AMARELO_MS);

    }
}