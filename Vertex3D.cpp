#include "incndef.h"
#include "v3deCore.h"
#include "v3deModel.h"

void OrbitTransform(Vector3 center, float radius, Vector3 angle, Vector3* ObjPos, Vector3* ObjRot) {
    Vector3 rad = {
            angle.x * (PI / 180),
            angle.y * (PI / 180),
            0
    };

    *ObjPos = {
         center.x + (-sin(rad.x) * radius),
         center.y + (sin(rad.y) * radius),
         center.z + (-cos(rad.x) * cos(rad.y) * radius)
    };

    Vector3 finalRot = lookAt(*ObjPos, center);

    *ObjRot = { finalRot.x,finalRot.y,ObjRot->z };
}

void WriteBox(char** screen, Vector2 RenderPos, int sizeX, int sizeY) {
    for (int y = 0; y < sizeY; y++) {
        for (int x = 0; x < sizeX; x++) {
            if (y == 0) {
                if (x == 0)
                    screenWrite(screen, { RenderPos.x + x,RenderPos.y + y }, "+", 0);
                else if (x == sizeX - 1)
                    screenWrite(screen, { RenderPos.x + x,RenderPos.y + y }, "+", 0);
                else
                    screenWrite(screen, { RenderPos.x + x,RenderPos.y + y }, "-", 0);
            }                                                                      
            else if (y == sizeY - 1) {                                             
                if (x == 0)                                                        
                    screenWrite(screen, { RenderPos.x + x,RenderPos.y + y }, "+", 0);
                else if (x == sizeX - 1)                                           
                    screenWrite(screen, { RenderPos.x + x,RenderPos.y + y }, "+", 0);
                else                                                               
                    screenWrite(screen, { RenderPos.x + x,RenderPos.y + y }, "-", 0);
            }                                                                      
            else {                                                                 
                if (x == 0 || x == sizeX - 1)                                      
                    screenWrite(screen, { RenderPos.x + x,RenderPos.y + y }, "|", 0);
            }
        }
    }
    
}

void saveModel(Object obj) {
    printf("[ Input model name ... ]\n> ");
    char filename[50];
    scanf("%s", filename);

    strcat(filename, ".v3d");

    FILE* file = fopen(filename, "w"); // 쓰기 모드로 파일 열기
    if (file == NULL) {
        fprintf(stderr, "[ Error : Can't open a file for writing. ]\n");
    }

    printf("[ %s ][ saving model... ]\n", filename);

    fprintf(file, "%lf %lf %lf\n", obj.transform.position.x, obj.transform.position.y, obj.transform.position.z);
    printf("[ SAVED ][ Position data saved ]\n");
    fprintf(file, "%lf %lf %lf\n", obj.transform.rotation.x, obj.transform.rotation.y, obj.transform.rotation.z);
    printf("[ SAVED ][ Rotation data saved ]\n");
    fprintf(file, "%d %d\n", obj.v_Length, obj.l_Length);
    printf("[ SAVED ][ v_data l_data saved ]\n");
    Sleep(500);

    for (int v = 0; v < obj.v_Length; v++) {
        fprintf(file, "%d %lf %lf %lf\n", v, obj.vertices[v].x, obj.vertices[v].y, obj.vertices[v].z);
        printf("[ SAVED ][ @%d vertex data saved ]\n", v);
        Sleep(1);
    }
    Sleep(300);
    for (int l = 0; l < obj.l_Length; l++) {
        fprintf(file, "%d %d %d\n", l, obj.vertexLine[l].a, obj.vertexLine[l].b);
        printf("[ SAVED ][ %d line data saved ]\n", l);
        Sleep(1);
    }

    fclose(file);

    printf("[ SUCCESS ][ >%s< Model saved and file closed successfully ]\n", filename);
}

void loadModel(Object* obj) {
    printf("[ Input model name ... ]\n> ");
    char filename[50];
    scanf("%s", filename);

    strcat(filename, ".v3d");

    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "[ Error : Can't open file for reading. ]\n");
        return;
    }

    fscanf(file, "%lf %lf %lf", &obj->transform.position.x, &obj->transform.position.y, &obj->transform.position.z);
    printf("[ LOADED ][ Position data: %lf, %lf, %lf ]\n", obj->transform.position.x, obj->transform.position.y, obj->transform.position.z);

    fscanf(file, "%lf %lf %lf", &obj->transform.rotation.x, &obj->transform.rotation.y, &obj->transform.rotation.z);
    printf("[ LOADED ][ Rotation data: %lf, %lf, %lf ]\n", obj->transform.rotation.x, obj->transform.rotation.y, obj->transform.rotation.z);

    fscanf(file, "%d %d", &obj->v_Length, &obj->l_Length);
    printf("[ LOADED ][ Vertex count: %d, Line count: %d ]\n", obj->v_Length, obj->l_Length);

    obj->vertices = (Vector3*)realloc(obj->vertices, obj->v_Length * sizeof(Vector3));
    obj->vertexLine = (Line*)realloc(obj->vertexLine, obj->l_Length * sizeof(Line));
    Sleep(500);

    for (int v = 0; v < obj->v_Length; v++) {
        int index;
        fscanf(file, "%d %lf %lf %lf", &index, &obj->vertices[v].x, &obj->vertices[v].y, &obj->vertices[v].z);
        printf("[ LOADED ][ Vertex @%d: %lf, %lf, %lf ]\n", index, obj->vertices[v].x, obj->vertices[v].y, obj->vertices[v].z);
        Sleep(1);
    }
    Sleep(300);
    for (int l = 0; l < obj->l_Length; l++) {
        int index;
        fscanf(file, "%d %d %d", &index, &obj->vertexLine[l].a, &obj->vertexLine[l].b);
        printf("[ LOADED ][ Line @%d: %d -> %d ]\n", index, obj->vertexLine[l].a, obj->vertexLine[l].b);
        Sleep(1);
    }

    fclose(file);
    printf("[ SUCCESS ][ Model loaded successfully from >%s< ]\n", filename);
}

double calculateFps() {
    static clock_t lastTime = 0;
    static int frameCount = 0;
    static double fps = 0;

    clock_t currentTime = clock();

    if (frameCount == 0) {
        lastTime = currentTime;
    }

    frameCount++;

    double elapsedTime = (double)(currentTime - lastTime) / 1000;

    if (elapsedTime >= 0.25f) {
        fps = frameCount / elapsedTime / 2;
        frameCount = 0;
        lastTime = currentTime;
    }

    return fps;
}

bool wait(double time) {
    static clock_t lastTime = 0;

    clock_t currentTime = clock();

    if (lastTime == 0) {
        lastTime = currentTime;
    }

    double elapsedTime = (double)(currentTime - lastTime) / CLOCKS_PER_SEC;

    if (elapsedTime >= time) {
        lastTime = 0;
        return true;
    }
    else {
        return false;
    }
}

int main() {
    system("MODE 85,55");
    RemoveCursor();

    int screenWidth = 80;
    int screenHeight = 40;

    char scene[50];
    strcpy(scene, "Main Menu");
    int selection = 0;

    Object gizmo;
    createGizmo(&gizmo);
    int UIstyle = 2;

    Object obj;
    createCube(&obj, 1.0f);
    //createTeapot(&obj);

    Vector3 objRot = v3Zero;
    Vector3 objPos = v3Zero;

    Camera gizmoCamera = { { v3Zero, v3Zero }, 90.0f };
    Camera mainCamera = { { v3Zero, v3Zero }, 90.0f };
    Vector3 cameraPos = { 0,0,-3 };
    Vector3 cameraRot = v3Zero;

    float cameraDistance = 3;
    Vector3 orbitCameraRot = v3Zero;

    int GizmoType = 0;
    bool running = true;

    while (running) {

        SetCurrentCursorPos({ 0,0 });

#pragma region KeyInput
        if (_kbhit()) {
            system("cls");
            char key = _getch();
            switch (key) {
#pragma region MoveMent
            case 'w':
                if (!strcmp(scene, "Edit Transform"))
                    objPos.y += 0.5f;
                else
                    orbitCameraRot.y += 5.0f;
                break;
            case 's':
                if (!strcmp(scene, "Edit Transform"))
                    objPos.y -= 0.5f;
                else
                    orbitCameraRot.y -= 5.0f;
                break;
            case 'a':
                if (!strcmp(scene, "Edit Transform"))
                    objPos.x -= 0.5f;
                else
                    orbitCameraRot.x += 5.0f;
                break;
            case 'd':
                if (!strcmp(scene, "Edit Transform"))
                    objPos.x += 0.5f;
                else
                    orbitCameraRot.x -= 5.0f;
                break;
            case 'q':
                if (!strcmp(scene, "Edit Transform"))
                    objPos.z += 0.5f;
                else
                    cameraRot.z += 5.0f;
                break;
            case 'e':
                if (!strcmp(scene, "Edit Transform"))
                    objPos.z -= 0.5f;
                else
                    cameraRot.z -= 5.0f;
                break;

            case 'i':
                if (!strcmp(scene, "Edit Transform"))
                    objRot.x += 5;
                else
                    objRot.y += 1.0f;
                break;
            case 'k':
                if (!strcmp(scene, "Edit Transform"))
                    objRot.x -= 5;
                else
                    objRot.y -= 1.0f;
                break;
            case 'j':
                if (!strcmp(scene, "Edit Transform"))
                    objRot.y -= 5;
                else
                    objRot.x -= 1.0f;
                break;
            case 'l':
                if (!strcmp(scene, "Edit Transform"))
                    objRot.y += 5;
                else
                    objRot.x += 1.0f;
                break;
            case 'u':
                if (!strcmp(scene, "Edit Transform"))
                    objRot.z -= 5;
                else
                    cameraRot.z -= 5.0f;
                break;
            case 'o':
                if (!strcmp(scene, "Edit Transform"))
                    objRot.z += 5;
                else
                    cameraRot.z += 5.0f;
                break;

            case 'r':
                cameraDistance -= 1;
                break;
            case 'f':
                cameraDistance += 1;
                break;
#pragma endregion
            case 'g':
                ++UIstyle;
                if (UIstyle == 3) UIstyle = 0;
                break;
            case UP:
                selection--;
                break;
            case DOWN:
                selection++;
                break;
            case ENTER:
                system("cls");
#pragma region MainMenu
                if (!strcmp(scene, "Main Menu")) {
                    switch (selection) {
                    case 0:
                        strcpy(scene, "Edit Vertex");
                        break;
                    case 1:
                        strcpy(scene, "Edit Line");
                        break;
                    case 2:
                        strcpy(scene, "Edit Transform");
                        break;
                    case 3:
                        strcpy(scene, "Save Model");
                        break;
                    case 4:
                        strcpy(scene, "Load Model");
                        break;
                    default:
                        selection = 0;
                        break;
                    }
                    break;
                }
#pragma endregion

#pragma region Edit Vertex
                if (!strcmp(scene, "Edit Vertex")) {
                    switch (selection) {
                    case 0:
                        strcpy(scene, "Add Vertex");
                        break;
                    case 1:
                        strcpy(scene, "Delete Vertex");
                        break;
                    case 2:
                        strcpy(scene, "Edit Existing Vertex");
                        break;
                    default:
                        selection = 0;
                        break;
                    }
                    break;
                }
                if (!strcmp(scene, "Delete Vertex")) {
                    strcpy(scene, "Delete Vertex_");
                    break;
                }
                if (!strcmp(scene, "Edit Existing Vertex")) {
                    strcpy(scene, "Edit Existing Vertex_");
                    break;
                }
#pragma endregion

#pragma region Edit Line
                if (!strcmp(scene, "Edit Line")) {
                    switch (selection) {
                    case 0:
                        strcpy(scene, "Add Line");
                        break;
                    case 1:
                        strcpy(scene, "Delete Line");
                        break;
                    case 2:
                        strcpy(scene, "Auto Line Connect");
                        break;
                    default:
                        selection = 0;
                        break;
                    }
                    break;
                }
#pragma endregion

            case 27: // ESC 키로 종료
                if (!strcmp(scene, "Main Menu")) {
                    running = false;
                }
                else {
                    //obj.transform.position = { 0,0,0 }; // 오브젝트 포지션 초기화
                    GizmoType = 0;
                    strcpy(scene, "Main Menu");
                }
                break;
            }
        }
#pragma endregion

        obj.transform.rotation.x += (objRot.x - obj.transform.rotation.x) * 0.1f;
        obj.transform.rotation.y += (objRot.y - obj.transform.rotation.y) * 0.1f;
        obj.transform.rotation.z += (objRot.z - obj.transform.rotation.z) * 0.1f;

        obj.transform.position.x += (objPos.x - obj.transform.position.x) * 0.1f;
        obj.transform.position.y += (objPos.y - obj.transform.position.y) * 0.1f;
        obj.transform.position.z += (objPos.z - obj.transform.position.z) * 0.1f;

        mainCamera.transform.position.x += (cameraPos.x - mainCamera.transform.position.x) * 0.1f;
        mainCamera.transform.position.y += (cameraPos.y - mainCamera.transform.position.y) * 0.1f;
        mainCamera.transform.position.z += (cameraPos.z - mainCamera.transform.position.z) * 0.1f;

        mainCamera.transform.rotation.x += (cameraRot.x - mainCamera.transform.rotation.x) * 0.1f;
        mainCamera.transform.rotation.y += (cameraRot.y - mainCamera.transform.rotation.y) * 0.1f;
        mainCamera.transform.rotation.z += (cameraRot.z - mainCamera.transform.rotation.z) * 0.1f;

        if(UIstyle) {
            printf("-----------------------------------------------------------------Model Transform\n");
            printf("cameraPos: %.2f %.2f %.2f\t", mainCamera.transform.position.x, mainCamera.transform.position.y, mainCamera.transform.position.z);
            printf("cameraRot: %.2f %.2f %.2f\n", mainCamera.transform.rotation.x, mainCamera.transform.rotation.y, mainCamera.transform.rotation.z);
            printf("objPos: %.2f %.2f %.2f   \t", obj.transform.position.x, obj.transform.position.y, obj.transform.position.z);
            printf("objRot: %.2f %.2f %.2f\n", obj.transform.rotation.x, obj.transform.rotation.y, obj.transform.rotation.z);
            printf("--------------------------------------------------------------------------------\n");
            printf("scene: %s", scene);
        }

        char** screen = screenInit(screenWidth, screenHeight);

        Render(screen, screenWidth, screenHeight, { 0,0 }, mainCamera, GizmoType, 1, obj);

        if (UIstyle) {
            WriteBox(screen, { 0,1 }, 80, 39);

            //WriteBox(screen, { 61,38 }, 17, 1);

            if(UIstyle == 2)
                Render(screen, screenWidth, screenHeight, { 0,0 }, mainCamera, GizmoType, 1, gizmo);

            WriteBox(screen, { 68,2 }, 10, 5);
            OrbitTransform({ 0,0,0 }, 1, orbitCameraRot, &gizmoCamera.transform.position, &gizmoCamera.transform.rotation);
            gizmoCamera.transform.rotation.z = mainCamera.transform.rotation.z;
            Render(screen, 10, 4, { 68,3 }, gizmoCamera, GizmoType, 1, gizmo);
            screenWrite(screen, { 70,2 }, "Origin", 0);

            WriteBox(screen, { 2,35 }, 18, 4);
            screenWrite(screen, { 5,35 }, "[Model Info]", 0);
            screenWrite(screen, { 4,36 }, "Vertices: %", 1, obj.v_Length);
            screenWrite(screen, { 4,37 }, "Lines: %", 1, obj.l_Length);

            static bool hintUI = true;
            if (wait(5)) hintUI = !hintUI;
            if(hintUI)  screenWrite(screen, { 3,39 }, "[ W A S D to move camera ]", 0);
            else screenWrite(screen, { 3,39 }, "[ Press G to hide UI ]", 0);
        }

        screenWrite(screen, { 2,1 }, "[Vertex 3D] v0.3", 0);
        screenWrite(screen, { 63,39 }, "[RenderFPS: %]", 1, (int)calculateFps());

        printScreen(screen, screenWidth, screenHeight, { 0,5 });

        
#pragma region DoAction
        //printf("--------------------------------RENDER--------------------------------\n");
        printf("\n------------------------------- Point Position : %d\n", selection);
        if (!strcmp(scene, "Main Menu")) {
            GizmoType = 0;

            OrbitTransform({ 0,0,0 }, cameraDistance, orbitCameraRot, & cameraPos, & cameraRot);

            if (UIstyle){
                if (selection == 0) printf(">");
                printf(" [ Edit Vertex ]\n");
                if (selection == 1) printf(">");
                printf(" [ Edit Line ]\n");
                if (selection == 2) printf(">");
                printf(" [ Edit Transform ]\n");
                if (selection == 3) printf(">");
                printf(" [ Save Model ]\n");
                if (selection == 4) printf(">");
                printf(" [ Load Model ]\n");
            }
        }
#pragma region Edit Vertex
        if (!strcmp(scene, "Edit Vertex")) {
            GizmoType = 2;
            OrbitTransform({ 0,0,0 }, cameraDistance, orbitCameraRot, &cameraPos, &cameraRot);

            if (selection == 0) printf(">");
            printf(" [ Add Vertex ]\n");
            if (selection == 1) printf(">");
            printf(" [ Delete Vertex ]\n");
            if (selection == 2) printf(">");
            printf(" [ Edit Existing Vertex ]\n");
        }
        if (!strcmp(scene, "Add Vertex")) {
            printf("[ Enter new vertex position ]\n");
            printf("[ Vector3 ][ X Y Z ]: ");
            Vector3 newVertexPos;
            scanf("%lf %lf %lf", &newVertexPos.x, &newVertexPos.y, &newVertexPos.z);
            ++obj.v_Length;
            obj.vertices = (Vector3*)realloc(obj.vertices, sizeof(Vector3) * (obj.v_Length));
            obj.vertices[obj.v_Length - 1] = newVertexPos;
            printf("\n[ New vertex added at { %.2f, %.2f, %.2f } ]\n",
                obj.vertices[obj.v_Length - 1].x, obj.vertices[obj.v_Length - 1].y, obj.vertices[obj.v_Length - 1].z);
            system("pause");
            strcpy(scene, "Main Menu");
            system("cls");
        }
        if (!strcmp(scene, "Delete Vertex")) {
            GizmoType = 1;
            printf("[ Select vertex to Delete ][ ESC to Main Menu ]\n");
            printf("[ Selected vertex: @%d ][ Enter to Delete ]\n", selection);
            if (selection >= 0 && selection < obj.v_Length) {
                OrbitTransform(obj.vertices[selection], cameraDistance, orbitCameraRot, &cameraPos, &cameraRot);
            }
            else {
                cameraPos = { 0,0,-5 };
            }
        }
        if (!strcmp(scene, "Delete Vertex_")) {
            if (selection >= 0 && selection < obj.v_Length) {
                for (int i = 0; i < obj.l_Length; ) {
                    if (obj.vertexLine[i].a == selection || obj.vertexLine[i].b == selection) {
                        for (int j = i; j < obj.l_Length - 1; j++) {
                            obj.vertexLine[j] = obj.vertexLine[j + 1];
                        }
                        obj.vertexLine = (Line*)realloc(obj.vertexLine, sizeof(Line) * (--obj.l_Length));
                    }
                    else {
                        i++;
                    }
                }

                for (int i = selection; i < obj.v_Length - 1; i++) {
                    obj.vertices[i] = obj.vertices[i + 1];
                }
                obj.vertices = (Vector3*)realloc(obj.vertices, sizeof(Vector3) * (--obj.v_Length));

                for (int i = 0; i < obj.l_Length; i++) {
                    if (obj.vertexLine[i].a > selection) obj.vertexLine[i].a--;
                    if (obj.vertexLine[i].b > selection) obj.vertexLine[i].b--;
                }
            }
            else {
                printf("[ ERROR : Invalid Value ]\n");
                system("pause");
                system("cls");
            }
            strcpy(scene, "Delete Vertex");
        }
        if (!strcmp(scene, "Edit Existing Vertex")) {
            printf("[ Select vertex to Edit ][ ESC to Main Menu ]\n");
            printf("[ Selected vertex: @%d ][ Enter to Edit ]\n", selection);
            if (selection >= 0 && selection < obj.v_Length) {
                OrbitTransform(obj.vertices[selection], cameraDistance, orbitCameraRot, &cameraPos, &cameraRot);
            }
            else {
                cameraPos = { 0,0,-5 };
            }
        }
        if (!strcmp(scene, "Edit Existing Vertex_")) {
            printf("[ @%d Vertex Selected ][ Enter the new position ]\n", selection);
            printf("[ Vector3 ][ X Y Z ]: ");
            Vector3 editPos;
            scanf("%lf %lf %lf", &editPos.x, &editPos.y, &editPos.z);
            printf("[ @%d Vertex Edited ]\n", selection);
            printf("{ %.2f, %.2f, %.2f } -> { %.2f, %.2f, %.2f }\n",
                obj.vertices[selection].x, obj.vertices[selection].y, obj.vertices[selection].z,
                editPos.x, editPos.y, editPos.z);

            obj.vertices[selection] = editPos;
            system("pause");
            strcpy(scene, "Edit Existing Vertex");
            system("cls");
        }
#pragma endregion

#pragma region Edit Line
        if (!strcmp(scene, "Edit Line")) {
            GizmoType = 1;
            OrbitTransform({ 0,0,0 }, cameraDistance, orbitCameraRot, &cameraPos, &cameraRot);

            if (selection == 0) printf(">");
            printf(" [ Add Line ]\n");
            if (selection == 1) printf(">");
            printf(" [ Delete Line ]\n");
            if (selection == 2) printf(">");
            printf(" [ Auto Line Connect ]\n");
        }
        if (!strcmp(scene, "Add Line")) {
            printf("[ Enter Vertex number to connect ]\n");
            printf("[ a b ]: ");
            Line newLine;
            scanf("%d %d", &newLine.a, &newLine.b);
            if (newLine.a < 0 || newLine.a >= obj.v_Length || newLine.b < 0 || newLine.b >= obj.v_Length) {
                printf("[ ERROR : Invalid Value ]\n");
                system("pause");
                strcpy(scene, "Main Menu");
                system("cls");
            }
            else {
                if (newLine.a < newLine.b) {
                    int temp = newLine.a;
                    newLine.a = newLine.b;
                    newLine.b = temp;
                }
                ++obj.l_Length;
                obj.vertexLine = (Line*)realloc(obj.vertexLine, sizeof(Line) * (obj.l_Length));
                obj.vertexLine[obj.l_Length - 1] = newLine;
                printf("\n[ New line connected with vertex @%d - @%d ]\n",
                    obj.vertexLine[obj.l_Length - 1].a, obj.vertexLine[obj.l_Length - 1].b);
                system("pause");
                strcpy(scene, "Main Menu");
                system("cls");
            }
        }
        if (!strcmp(scene, "Delete Line")) {
            printf("[ Enter Vertex number to disconnect ][ ESC to Main Menu ]\n");
            printf("[ a b ]: ");
            Line oldLine;
            scanf("%d %d", &oldLine.a, &oldLine.b);
            if (oldLine.a < 0 || oldLine.a >= obj.v_Length || oldLine.b < 0 || oldLine.b >= obj.v_Length) {
                printf("[ ERROR : Invalid Value ]\n");
                system("pause");
                strcpy(scene, "Main Menu");
                system("cls");
            }
            else {
                for (int i = 0; i < obj.l_Length; i++) {
                    if ((obj.vertexLine[i].b == oldLine.a && obj.vertexLine[i].a == oldLine.b)) {
                        int temp = oldLine.a;
                        oldLine.a = oldLine.b;
                        oldLine.b = temp;
                    }
                    if ((obj.vertexLine[i].a == oldLine.a && obj.vertexLine[i].b == oldLine.b)) {
                        for (int j = i; j < obj.l_Length - 1; j++)
                            obj.vertexLine[j] = obj.vertexLine[j + 1];
                        obj.vertexLine = (Line*)realloc(obj.vertexLine, sizeof(Line) * --obj.l_Length);
                    }
                }
            }
            printf("\n[ Line @%d - @%d disconnected ]\n", oldLine.a, oldLine.b);
            system("pause");
            strcpy(scene, "Main Menu");
            system("cls");
        }
        if (!strcmp(scene, "Auto Line Connect")) {
            obj.l_Length = obj.v_Length - 1;
            obj.vertexLine = (Line*)realloc(obj.vertexLine, sizeof(Line) * obj.l_Length);

            for (int i = 0; i < obj.l_Length; i++) {
                obj.vertexLine[i] = { i,i + 1 };
            }
            printf("\n[ Line auto connected ]\n");
            system("pause");
            strcpy(scene, "Main Menu");
            system("cls");
        }
#pragma endregion

#pragma region Edit Transform
        if (!strcmp(scene, "Edit Transform")) {
            printf("[ Position : W,A,S,D - Q,E ]\n[ Rotation : I,J,K,L - U,O ]\n[ ESC to Main Menu ]\n");
        }
#pragma endregion

#pragma region Save Model
        if (!strcmp(scene, "Save Model")) {
            saveModel(obj);
            system("pause");
            strcpy(scene, "Main Menu");
            system("cls");
        }
#pragma endregion

#pragma region Load Model
        if (!strcmp(scene, "Load Model")) {
            loadModel(&obj);
            objRot = obj.transform.rotation;
            objPos = obj.transform.position;
            system("pause");
            strcpy(scene, "Main Menu");
            system("cls");
        }
#pragma endregion

#pragma endregion

        for (int i = 0; i < screenHeight; i++) {
            free(screen[i]);
        }
        free(screen);
    }

    free(obj.vertices);
    free(obj.vertexLine);

    return 0;
}