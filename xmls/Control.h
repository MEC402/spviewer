
#ifndef CONTROL_H_
#define CONTROL_H_

const std::string ROTATE_UP_ID = "rotate_up";
const std::string ROTATE_DOWN_ID = "rotate_down";
const std::string ROTATE_LEFT_ID = "rotate_left";
const std::string ROTATE_RIGHT_ID = "rotate_right";
const std::string MOVE_FORWARD_ID = "move_forward";
const std::string MOVE_BACKWARD_ID = "move_backward";
const std::string MOVE_LEFT_ID = "move_left";
const std::string MOVE_RIGHT_ID = "move_right";
const std::string ZOOM_IN_ID = "zoom_in";
const std::string ZOOM_OUT_ID = "zoom_out";
const std::string FOV_INCREASE_ID = "FOV_increase";
const std::string FOV_DECREASE_ID = "FOV_decrease";
const std::string RESET_VALUES_ID = "reset_values";
const std::string SWAP_EYES_ID = "swap_eyes";
const std::string LEFT_SPHERE_ROTATION_INCREASE_ID = "left_sphere_rotation_increase";
const std::string LEFT_SPHERE_ROTATION_DECREASE_ID = "left_sphere_rotation_decrease";
const std::string LEFT_SPHERE_ROTATION_DISPLAY_TOGGLE_ID = "left_sphere_rotation_display_toggle";
const std::string LEFT_SPHERE_ROTATION_SAVE_VALUE_ID = "left_sphere_rotation_save_value";
const std::string MENU_TOGGLE_ID = "menu_toggle";
const std::string REF_GRID_TOGGLE_ID = "ref_grid_toggle";
const std::string RAM_DISPLAY_TOGGLE_ID = "RAM_display_toggle";
const std::string EYE_DISTANCE_INCREASE = "eye_distance_increase";
const std::string EYE_DISTANCE_DECREASE = "eye_distance_decrease";
const std::string STEREO_PANO_TOGGLE = "stereo_pano_toggle";
const std::string STEREO_LEFT_RIGHT_SPLIT = "stereo_left_right_split";
const std::string STEREO_TOP_BOTTOM_SPLIT = "stereo_top_bottom_split";
const std::string STEREO_ANAGLYPHIC_SPLIT = "stereo_anaglyphic_split";


  //create file
  // If CONTROLSCHANGED = TRUE || !(HTML FILE EXISTS)
    //write to file:
      //std::cout << "<html><head></head><body>" << std::endl;
      //for(a in controlArray){
      //	std::cout << a << ": " << Controls::getControlKey(a) << std::endl;
      //}
      //std::cout << "</body></html>" << std::endl;
    // Save File
  // else
    // Open File


class Controls {
  
public:
  
  Controls(){};
  ~Controls(){};


  void addControl(std::string id, std::string keyvalue){ controls[id] = keyvalue; };
  
  std::map<std::string, std::string> getControlMap(){ return controls; };
  
  
  char getControlKey(std::string id){
    
    std::map<std::string, std::string>::const_iterator pos;
    pos = controls.find(id);
    
    if(pos == controls.end()){
      return '\0';
      
    } else {
      return (pos->second).at(0);
      
    }

  };
  
  
  // Default Controls
  void setDefaultControls(){
    
    addControl(ROTATE_UP_ID, "w");
    addControl(ROTATE_DOWN_ID, "s");
    addControl(ROTATE_LEFT_ID, "a");
    addControl(ROTATE_RIGHT_ID, "d");
    addControl(MOVE_FORWARD_ID, "l");
    addControl(MOVE_BACKWARD_ID, "L");
    addControl(MOVE_LEFT_ID, "k");
    addControl(MOVE_RIGHT_ID, "K");
    addControl(ZOOM_IN_ID, "q");
    addControl(ZOOM_OUT_ID, "z");
    addControl(FOV_INCREASE_ID, "[");
    addControl(FOV_DECREASE_ID, "]");
    addControl(RESET_VALUES_ID, "e");
    addControl(SWAP_EYES_ID, "t");
    addControl(LEFT_SPHERE_ROTATION_INCREASE_ID, "p");
    addControl(LEFT_SPHERE_ROTATION_DECREASE_ID, "P");
    addControl(LEFT_SPHERE_ROTATION_DISPLAY_TOGGLE_ID, "G");
    addControl(LEFT_SPHERE_ROTATION_SAVE_VALUE_ID, "g");
    addControl(MENU_TOGGLE_ID, "m");
    addControl(REF_GRID_TOGGLE_ID, "R");
    addControl(RAM_DISPLAY_TOGGLE_ID, "I");
    addControl(EYE_DISTANCE_INCREASE, "1");
    addControl(EYE_DISTANCE_DECREASE, "2");
    addControl(STEREO_PANO_TOGGLE, "3");
    addControl(STEREO_LEFT_RIGHT_SPLIT, "v");
    addControl(STEREO_TOP_BOTTOM_SPLIT, "h");
    addControl(STEREO_ANAGLYPHIC_SPLIT, "b");
   
  }


private:
  
  std::map<std::string, std::string> controls;
  
};
  
#endif