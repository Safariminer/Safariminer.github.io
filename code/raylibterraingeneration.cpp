/*
* raylib Terrain Generation
* written by Safariminer
* 
* Program length without this comment: 50 lines
* Description: Generates a terrain and a texture to go with said 
*              terrain at runtime in 50 lines of code.
* 
* Benefits: No external file needed nor generated. Might be
*           useful for demos(although I doubt it because of the 
*           size of raylib)
* 
* Drawbacks: The terrain generating function does not unload its
*            images from memory. Calling it repeatedly could
*            cause memory leaks.
* 
* Compromises: 50 lines means 50 lines. Not 50 function calls.
*              Of course, this seems a bit like cheating, but it
*              is what it is. Feel free to rewrite this in 50
*              function calls. I dare you.
* 
* Dependencies: raylib.
* ---
* Personal site: safari.is-probably.gay
* Main email: safariminer16@gmail.com
* Github: github.com/safariminer
* Gitlab: gitlab.com/safariminer
* PREFERED: Windows Live Messenger(2009): safariminer@escargot.chat
* PREFERED: Trackmania Nations Forever: safariminer
* PREFERED: ManiaPlanet: safariminer
* 
* License: WTFPL
*/
#include <raylib.h>
#define SENSITIVITY 0.1f

Model GenerateTerrain() {
	Model terrain;
	Image heightmap = GenImagePerlinNoise(1024, 1024, 0, 0, 1);
	ImageColorContrast(&heightmap, 2);
	terrain = LoadModelFromMesh(GenMeshHeightmap(heightmap, {1024, 1024, 1024}));
	Image grass = GenImageWhiteNoise(1024, 1024, 0.5f), stone = GenImageCellular(1024, 1024, 8), snow = GenImageWhiteNoise(1024, 1024, 0.1f), water = GenImageCellular(1024, 1024, 8), sand = GenImagePerlinNoise(1024, 1024, 0, 0, 1);
	
	ImageColorTint(&grass, GREEN); ImageBlurGaussian(&grass, 2);
	ImageBlurGaussian(&stone, 2); ImageColorBrightness(&stone, 50);
	ImageColorTint(&water, DARKBLUE); ImageBlurGaussian(&water, 2);
	ImageBlurGaussian(&snow, 2); ImageColorBrightness(&snow, 200);
	ImageBlurGaussian(&sand, 3); ImageColorTint(&sand, YELLOW); ImageColorBrightness(&sand, 50);

	Image overlayingtexture = GenImageColor(1024, 1024, BLACK);
	for (int y = 0; y < 1024; y++) {
		for (int x = 0; x < 1024; x++) {
			int height = GetImageColor(heightmap, x, y).r;
			Color grasspixel = GetImageColor(grass, x, y), stonepixel = GetImageColor(stone, x, y), waterpixel = GetImageColor(water, x, y), snowpixel = GetImageColor(snow, x, y), sandpixel = GetImageColor(sand, x, y);
			ImageDrawPixel(&overlayingtexture, x, y, height < 50 ? waterpixel : height < 60 ? sandpixel : height < 100 ? grasspixel : height < 150 ? stonepixel : snowpixel);
		}
	}
	terrain.materials->maps[0].texture = LoadTextureFromImage(overlayingtexture);
	return terrain;
}

int main() {
	InitWindow(1280, 720, "raylib Terrain Generation");
	Camera camera = { 0 };
	camera.position = { 0.0f, 5.0f, 0.0f }, camera.target = { 4.0f, 1.0f, 4.0f }, camera.up = { 0.0f, 1.0f, 0.0f }, camera.fovy = 90.0f, camera.projection = CAMERA_PERSPECTIVE;
	Model terrain = GenerateTerrain();
	DisableCursor();
	while (!WindowShouldClose()) {
		UpdateCameraPro(&camera,
			{ (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) * 0.1f - (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) * 0.1f,
			  (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) * 0.1f - (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) * 0.1f,
			  (IsKeyDown(KEY_SPACE)) * 0.1f - (IsKeyDown(KEY_LEFT_CONTROL)) * 0.1f
			},
			{ GetMouseDelta().x * SENSITIVITY, GetMouseDelta().y * SENSITIVITY, 0.0f}, GetMouseWheelMove() * 2.0f);
		ClearBackground(SKYBLUE);
		BeginMode3D(camera);
		DrawModel(terrain, { 0,0,0 }, 1, WHITE);
		EndMode3D();
		DrawFPS(0,0);
		EndDrawing();
	}
	CloseWindow();
}
