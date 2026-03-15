
#include <box2d/box2d.h>
#include <iostream>

void CreateAndRunPhysics() {
    // ==========================================
    // 第一階段：建立物理世界 (造物工廠)
    // ==========================================
    b2Vec2 gravity(0.0f, -9.8f); // 設定重力向下
    
    // 實例化世界 (在實際遊戲中，這通常會是被 new 出來的指標，存活在 LevelManager 裡)
    b2World world(gravity); 

    // ==========================================
    // 第二階段：建立組合件的「骨架」(Body)
    // ==========================================
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;       // 這是一個會往下掉的動態物體
    bodyDef.position.Set(0.0f, 10.0f);   // 讓它出生在半空中 (X:0, Y:10)
    bodyDef.angle = 0.2f;                // 稍微給它一點傾斜角度，掉下來比較有趣

    // 請世界幫我們把這個空骨架做出來
    b2Body* tBlockBody = world.CreateBody(&bodyDef);

    // ==========================================
    // 第三階段：打造並組裝「零件」(Fixtures)
    // ==========================================
    
    // 準備一個共用的夾具設定檔 (材質屬性)
    b2FixtureDef fixtureDef;
    fixtureDef.density = 1.0f;    // 密度 (決定重量)
    fixtureDef.friction = 0.5f;   // 摩擦力
    fixtureDef.restitution = 0.2f;// 彈性 (掉到地上稍微彈一下)

    // 🌟 零件 A：頂部的橫向木條
    b2PolygonShape topBarShape;
    // 設定半寬 2.0, 半高 0.5。並且把它的中心點往「上」偏移 1.0
    topBarShape.SetAsBox(2.0f, 0.5f, b2Vec2(0.0f, 1.0f), 0.0f);
    
    fixtureDef.shape = &topBarShape; // 將形狀綁定到夾具設定檔
    tBlockBody->CreateFixture(&fixtureDef); // 鎖上第一個零件！

    // 🌟 零件 B：垂直的把手/身體
    b2PolygonShape stemShape;
    // 設定半寬 0.5, 半高 1.5。並且把它的中心點往「下」偏移 1.0
    stemShape.SetAsBox(0.5f, 1.5f, b2Vec2(0.0f, -1.0f), 0.0f);
    
    fixtureDef.shape = &stemShape; // 替換成第二個形狀
    tBlockBody->CreateFixture(&fixtureDef); // 鎖上第二個零件！

    // ==========================================
    // 第四階段：放入世界並開始模擬 (遊戲迴圈)
    // ==========================================
    
    // 模擬遊戲的 Update 迴圈跑 60 幀 (大約 1 秒鐘)
    float timeStep = 1.0f / 60.0f;
    int velocityIterations = 6;
    int positionIterations = 2;

    std::cout << "開始模擬 T 型積木掉落...\n";
    for (int i = 0; i < 60; ++i) {
        // 推進物理世界的時間
        world.Step(timeStep, velocityIterations, positionIterations);

        // 取得 T 型積木目前的位置與角度
        b2Vec2 position = tBlockBody->GetPosition();
        float angle = tBlockBody->GetAngle();

        // 每 10 幀印出一次狀態，觀察它怎麼掉落
        if (i % 10 == 0) {
            std::cout << "Frame " << i 
                      << " | 位置: (" << position.x << ", " << position.y << ")"
                      << " | 角度: " << angle << "\n";
        }
    }
}



// ==========================================
// 1. 先在桌面上畫好「零件設計圖」(FixtureDef)
// ==========================================
b2PolygonShape boxShape;
boxShape.SetAsBox(1.0f, 1.0f); // 形狀：2x2 的正方形

b2FixtureDef fixtureDef;
fixtureDef.shape = &boxShape;  // 綁定形狀
fixtureDef.density = 5.0f;     // 設定密度
fixtureDef.friction = 0.8f;    // 設定摩擦力
fixtureDef.restitution = 0.1f; // 設定彈性

// ==========================================
// 2. 接著畫好「骨架設計圖」(BodyDef)
// ==========================================
b2BodyDef bodyDef;
bodyDef.type = b2_dynamicBody;
bodyDef.position.Set(0.0f, 10.0f);

// ==========================================
// 3. 最後，把設計圖交給工廠 (World) 去生產！
// ==========================================
// A. 先產生骨架
b2Body* myBody = world->CreateBody(&bodyDef); 

// B. 再把剛剛早就準備好的零件設計圖，掛到這個骨架上
myBody->CreateFixture(&fixtureDef);