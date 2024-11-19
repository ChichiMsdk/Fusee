#include "init.h"
#include "fusee.h"
#include "event.h"
#include "neurone.h"

Instance gInst = {.width = 1500, .height = 800, .running = 1};

float gravity = 0.1f;

double input[INPUT_SIZE];
double hidden[HIDDEN_SIZE];
double output[OUTPUT_SIZE];
double input_weights[INPUT_SIZE][HIDDEN_SIZE];
double output_weights[HIDDEN_SIZE][OUTPUT_SIZE];

int
main(int argc, char** ppArgv)
{
	SDL_Event	e;

	InitSDL(&gInst);

	Fusee fusee = {0};
	FuseeInit(&fusee);

	size_t layerCount = 2;
	size_t inputCount = 4;
	size_t outputCount = 4;
	IntArray perLayerCount = {
		.count = 1,
		.pArray = malloc(sizeof(int) * inputCount),
	};

	initialize_weights(input_weights, INPUT_SIZE, HIDDEN_SIZE);
	initialize_weights(output_weights, HIDDEN_SIZE, OUTPUT_SIZE);
	// Perform feedforward

	while (gInst.running)
	{
		Event(&e, &fusee);

		SDL_SetRenderDrawColor(gInst.pRenderer, 0, 0, 0, 255);
		SDL_RenderClear(gInst.pRenderer);
		float rotationSpeed = 0.3;

		if (output[0] > 0.5 && fusee.position.y > 0 && fusee.position.y < gInst.height - fusee.box.h)
		{
			fusee.angle -= rotationSpeed;
			fusee.fire.angle -= rotationSpeed;
			if (fusee.angle < 0)
				fusee.angle += 360;
			if (fusee.fire.angle < 0)
				fusee.fire.angle += 360;
		}
		if (output[1] > 0.5 && fusee.position.y > 0 && fusee.position.y < gInst.height - fusee.box.h)
		{
			fusee.angle += rotationSpeed;
			fusee.fire.angle += rotationSpeed;
			if (fusee.angle >= 360)
				fusee.angle -= 360;
			if (fusee.fire.angle >= 360)
				fusee.fire.angle -= 360;
		}
		if (output[2] > 0.5)
		{
			float radians = fusee.angle * M_PI / 180.0f;
			fusee.velocity.x += fusee.power * sin(radians);
			fusee.velocity.y -= fusee.power * cos(radians);

			radians = fusee.angle * M_PI / 180.0f;
			fusee.fire.velocity.x += fusee.power * sin(radians);
			fusee.fire.velocity.y -= fusee.power * cos(radians);
			fusee.firing = true;
		}

		FuseeUpdate(&fusee, gravity);
		input[0] = fusee.position.x;
		input[1] = fusee.position.y;
		input[2] = fusee.velocity.x;
		input[3] = fusee.velocity.y;
		input[4] = fusee.angle;
		feedforward(input, hidden, output, input_weights, output_weights);

		for (int i = 0; i < OUTPUT_SIZE; i++) 
			printf("%f\n", output[i]);

		FuseeDraw(&fusee);

		SDL_RenderPresent(gInst.pRenderer);
		SDL_Delay(16);
	}
		IMG_Quit();
		SDL_DestroyTexture(fusee.fire.fireTexture);
		SDL_DestroyTexture(fusee.fuseeTexture);
		SDL_DestroyRenderer(gInst.pRenderer);
		SDL_DestroyWindow(gInst.pWin);
		SDL_Quit();

	return 0;
}
