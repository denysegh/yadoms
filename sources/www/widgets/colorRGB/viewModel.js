widgetViewModelCtor =

/**
 * Create a colorRGB ViewModel
 * @constructor
 */
function colorRGBViewModel() {
     this.colorpicker = null;
     this.WidgetWidth  = 156;
     this.WidgetHeight = 120;
     
    /**
     * Initialization method
     * @param widget widget class object
     */     
    this.initialize = function () {
        self = this;
       
        //we configure the toolbar
        this.widgetApi.toolbar({
            activated: true,
            displayTitle: true,
            batteryItem: false
        });
    };

    this.createPicker = function (preselectedColor) {
        self.colorpicker = this.widgetApi.find(".picker-canvas").colorpicker({
            customClass: 'colorpicker-size',
            hexNumberSignPrefix: false,
            sliders: {
                saturation: {
                    maxLeft: 156,
                    maxTop: 120
                },
                hue: {
                    maxTop: 120
                },
                alpha: {
                    maxTop: 120
                }
            },
            color: '#000000',
            format: 'rgb',

            colorSelectors: preselectedColor,
            // remove the dropdown-menu class to disable the border
            template : "<div class='colorpicker'><div class='colorpicker-saturation'><i><b></b></i></div><div class='colorpicker-hue'><i></i></div><div class='colorpicker-alpha'><i></i></div><div class='colorpicker-color'><div /></div><div class='colorpicker-selectors'></div></div>",
            container: true,
            inline: true
        });
        
        // capture the event changeColor
        self.colorpicker.unbind('changeColor').bind('changeColor', self.changeColorButtonClick());
    };
    
    // function called when the color changed
    this.changeColorButtonClick = function () {
           var self = this;
           return function (e) {
            console.log ("color changed ! :", e.color.toHex());
            KeywordManager.sendCommand(self.widget.configuration.device.keywordId, e.color.toHex()/*.slice(1)*/.toString());      
        };
    };

/*
https://stackoverflow.com/questions/21117842/converting-an-rgbw-color-to-a-standard-rgb-hsb-rappresentation
M = max(Ri,Gi,Bi)
m = min(Ri,Gi,Bi)

Wo = if (m/M < 0.5) use ( (m*M) / (M-m) ) else M 
Q = 255
K = (Wo + M) / m
Ro = floor( [ ( K * Ri ) - Wo ] / Q )
Go = floor( [ ( K * Gi ) - Wo ] / Q )
Bo = floor( [ ( K * Bi ) - Wo ] / Q )
*/    
    
    this.convertRGBtoRGBW = function (red, green, blue) {
       var max = Math.max(red, green, blue);
       var min = Math.min(red, green, blue);
       var W0 = 0;
       
       if (min/max <0.5)
          W0 = (min*max)/(max-min);
       else
          W0 = max;
       
       var Q = 255;
       
       value["white"] = (W0 + max) / min;
       value["red"]   = Math.floor (((K*red)-W0)/Q);
       value["green"] = Math.floor (((K*green)-W0)/Q);
       value["blue"]  = Math.floor (((K*blue)-W0)/Q);
       
       return value;
    };
    
    this.configurationChanged = function () {
       var self = this;
       var preselectedColor = null;

       if ((isNullOrUndefined(this.widget)) || (isNullOrUndefinedOrEmpty(this.widget.configuration)))
          return;
       
       //we register keyword new acquisition
       self.widgetApi.registerKeywordAcquisitions(self.widget.configuration.device.keywordId);       
       
       // destroy the precedent colorPicker if any
       // it's the only solution, to create/delete preselected colors
       if (!isNullOrUndefined(self.colorpicker)) self.colorpicker.colorpicker('destroy');
       
       if (!(isNullOrUndefinedOrEmpty(self.widget.configuration.colorsSelection)))
       {
          if (self.widget.configuration.colorsSelection.length>0) preselectedColor={};
          self.widget.configuration.colorsSelection.forEach(function (item) {
              var newProperties={};
              newProperties[item.content.name] = item.content.color;
              $.extend(preselectedColor, newProperties);
          });
       }
       
       self.createPicker(preselectedColor);
       self.resized();
    };
    
    this.changeCss = function(width, height) {
      $('.widget-colorRGB .colorpicker-saturation').css('height', height+'px');
      $('.widget-colorRGB .colorpicker-saturation').css('width', width+'px');
      $('.widget-colorRGB .colorpicker-saturation').css('background-size', width+'px '+height+'px');
      $('.widget-colorRGB .colorpicker-hue').css('height', height+'px');
      $('.widget-colorRGB .colorpicker-hue').css('background-size', '15px '+height+'px');
      $('.widget-colorRGB .colorpicker-alpha').css('height', height+'px');
      $('.widget-colorRGB .colorpicker-alpha').css('background-size', '14px '+height+'px');
      $('.widget-colorRGB .colorpicker-color').css('background-size', '14px '+height+'px');
      $('.widget-colorRGB .colorpicker-color div').css('background-size', '14px '+height+'px');
      $('.widget-colorRGB .colorpicker-selectors').css('width', width+15+'px');
    };

   this.resized = function() 
   {
      var self = this;
      self.WidgetWidth = this.widget.getWidth()-34;
      
      // If no colors preselected, we use all the space
      if (isNullOrUndefined(self.colorpicker.data('colorpicker').options.colorSelectors))
         self.WidgetHeight = this.widget.getHeight()-57;
      else
         self.WidgetHeight = this.widget.getHeight()-90;
      
      // Update sliders values
      self.colorpicker.data('colorpicker').options.sliders.saturation.maxLeft = self.WidgetWidth;
      self.colorpicker.data('colorpicker').options.sliders.saturation.maxTop = self.WidgetHeight;
      
      self.colorpicker.data('colorpicker').options.sliders.hue.maxTop = self.WidgetHeight;
      self.colorpicker.data('colorpicker').options.sliders.alpha.maxTop = self.WidgetHeight;      
      
      self.changeCss(self.WidgetWidth, self.WidgetHeight);
      
      self.colorpicker.colorpicker('update');
   };
    
    /**
    * New acquisition handler
    * @param keywordId keywordId on which new acquisition was received
    * @param data Acquisition data
    */
    this.onNewAcquisition = function (keywordId, data) {
        var self = this;
        
        if (keywordId === self.widget.configuration.device.keywordId) {
          
          // unbind the changeColor, otherwise, firea 'changeColor'
          self.colorpicker.unbind('changeColor');
          self.colorpicker.colorpicker('setValue', data.value);
          self.colorpicker.unbind('changeColor').bind('changeColor', self.changeColorButtonClick());
        }
    };
};