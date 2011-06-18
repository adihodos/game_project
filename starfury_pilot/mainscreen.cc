#include "precompiled.h"
#include "d2drenderer.h"
#include "gamefont.h"
#include "game_resourcecache.h"
#include "lazy_unique_instance.h"
#include "mainscreen.h"
#include "uipushbutton.h"

MainGameScreen::MainGameScreen( 
  ScreenManager* smgr, float x, float y, float bx, float by, 
  IGameScreen* parent /* = nullptr */, bool active /* = true */ 
  )
  : IGameScreen(smgr, x, y, bx, by, parent, active), resprovider_() {}

bool MainGameScreen::Initialize() {
  assert(!resprovider_);
  resprovider_ = base::LazyUniqueInstance<GameResourceCache>::Get();
  testbtn_.reset(
    new UIPushButton(1, 100, 100, 200, 50, 
                     D2D1::ColorF(D2D1::ColorF::Black),
                     D2D1::ColorF(D2D1::ColorF::Orange),
                     D2D1::ColorF(D2D1::ColorF::DarkOliveGreen),
                     D2D1::ColorF(D2D1::ColorF::Chocolate),
                     D2D1::ColorF(D2D1::ColorF::Gray),
                     GameFontData(L"Arial", 32.0f, GameFontData::GFD_FW_HEAVY),
                     nullptr, 2.0f, L"Apasa-ma"));

  return true;
}

bool MainGameScreen::KeyPressed(KeyboardEventArgs*) {
  return true;
}

bool MainGameScreen::KeyReleased(KeyboardEventArgs*) {
  return true;
}

bool MainGameScreen::LeftButtonDown(MouseEventArgs* args) {
  return testbtn_->LeftButtonDown(args);
}

bool MainGameScreen::LeftButtonUp(MouseEventArgs* args) {
  return testbtn_->LeftButtonUp(args);
}

bool MainGameScreen::RightButtonDown(MouseEventArgs* args) {
  return false;
}

bool MainGameScreen::RightButtonUp(MouseEventArgs* args) {
  return false;
}

bool MainGameScreen::MouseMoved(MouseEventArgs* args) {
  return testbtn_->MouseMoved(args);
}

void MainGameScreen::Draw(Direct2DRenderer* renderer) {
  ID2D1HwndRenderTarget* rtarget = 
    static_cast<ID2D1HwndRenderTarget*>(renderer->GetRendererTarget());
  rtarget->SetTransform(D2D1::Matrix3x2F::Identity());

  ID2D1SolidColorBrush* bluebrush = resprovider_->GetSolidColorBrushHandle(
      D2D1::ColorF(D2D1::ColorF::DeepSkyBlue));

  ID2D1SolidColorBrush* blkbrush = resprovider_->GetSolidColorBrushHandle(
    D2D1::ColorF(D2D1::ColorF::Black));

  D2D1_RECT_F baserect(
    D2D1::RectF(topleft_.x_, topleft_.y_, bottomright_.x_, bottomright_.y_));

  rtarget->FillRectangle(baserect, blkbrush);

  gfx::matrix3X3 mtx_scale(gfx::matrix3X3::scale(0.95f, 0.95f));

  gfx::vector2D vtopleft = mtx_scale * topleft_;
  gfx::vector2D vbotright = mtx_scale * bottomright_;

  baserect.left = vtopleft.x_;
  baserect.top = vtopleft.y_;
  baserect.right = vbotright.x_;
  baserect.bottom = vbotright.y_;

  D2D1_ROUNDED_RECT rrect(D2D1::RoundedRect(baserect, 8.0f, 8.0f));
  /*float old_opacity = bluebrush->GetOpacity();
  bluebrush->SetOpacity(0.25f);*/
  rtarget->FillRoundedRectangle(rrect, bluebrush);
  //bluebrush->SetOpacity(old_opacity);

  testbtn_->Draw(renderer);
}