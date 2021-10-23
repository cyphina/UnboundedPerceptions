#include "MyProject.h"
#include "BaseHero.h"
#include "AutomationCommon.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FHeroAttTest, "Gameplay.Hero Attribute Test", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

namespace BaseHeroTest
{
   ABaseHero* testHero;
   UWorld*    world;

   bool Setup()
   {
    /*  if (GEngine) {
         if (AutomationOpenMap("/Game/RTS_Tutorial/Maps/TheIntroduction")) {
            world    = GEngine->GetWorldContextFromPIEInstance(0)->World();
            testHero = world->SpawnActor<ABaseHero>(ABaseHero::StaticClass(), FTransform(), FActorSpawnParameters());
            return true;
         }
      }*/
      return false;
   }
} // namespace BaseHeroTest

using namespace BaseHeroTest;

bool FHeroAttTest::RunTest(const FString& params)
{
   if (BaseHeroTest::Setup()) {
      // TestNotNull(TEXT("GEngine is not null"), GEngine);
      //
      // int        initialHeroAttPoints = testHero->attPoints;
      // EAttributes attToTest            = EAttributes::Agility;
      // int        initialAttValue      = testHero->GetAttributeAdjValue(attToTest);
      //
      // testHero->ChangeAttribute(EAttributes::Agility, true);
      // TestEqual("AttributePoints", testHero->attPoints, initialHeroAttPoints - 1);
      // TestEqual("AttributeValue", testHero->GetAttributeAdjValue(attToTest), initialAttValue + 1);
      // testHero->ChangeAttribute(EAttributes::Agility, false);
      // TestEqual("AttributePoints", testHero->attPoints, initialHeroAttPoints);
      // TestEqual("AttributeValue", testHero->GetAttributeAdjValue(attToTest), initialAttValue);
      // return true;
   }
   return false;
}
