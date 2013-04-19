unit shared;

interface
uses graphics;

const FlatButtons=false;
      PAGES_MAIN=0;
      PAGES_CHANNELS=1;
      PAGES_LEVELS=2;
      PAGES_IMAGES=3;
var clADbar,clADbarText,clADbarSelected,clADbarSelectedText:TColor;

implementation

begin
  clADbar:=clSilver;
  clADbarText:=clBlack;
  clADbarSelected:=ClHighLight;
  clADbarSelectedText:=clHighLightText;
end.
