var edu_stanford_bricolage = {
	
 	isInline: function(node) {
		return jQuery(node).css("display") == undefined  || jQuery(node).css("display") == "inline";
	},

	isText: function(node) {
		return node.nodeType == Node.TEXT_NODE && jQuery.trim(node.nodeValue) != "";
	},

	needsInlineWrap: function(element) {

		var containsText = false;
		var containsBlock = false;

		for (var i=0; i < element.childNodes.length; i++) {
			var node = element.childNodes[i];

			if(this.isText(node)) {
					containsText = true;
			}

			if(!this.isInline(node)) {
				containsBlock = true;
			}

		};

		return containsText && containsBlock;

	},

	createWrap: function() {
		var wrap = document.createElement("span");

		for(var i in this.bricolage_css_properties) {
			var prop = this.bricolage_css_properties[i];
			wrap.style[prop] = "inherit";
		}
		
		wrap.style.display = "inline";
		wrap.style.float = "none";
		wrap.style.top = 0;
		wrap.style.left = 0;

		return wrap;
	},

	inlineWrap: function(element) {

		if(this.needsInlineWrap(element)) {

			var wrap = this.createWrap();

			for (var i=0; i < element.childNodes.length; i++) {

				var node = element.childNodes[i];

				if(this.isInline(node)) {
					// Append child removes it from this previous parent, if it had one
					wrap.appendChild(node);
					i--;
				} else {
					if(wrap.childNodes.length > 0) {
						element.insertBefore(wrap, node);
						wrap = this.createWrap();
					}
				}
			}

			if(wrap.childNodes.length > 0) {
				element.appendChild(wrap, node);
			}

		}

		/* Recurse second to avoid invalid nodes being passed in */
		for (var i=0; i < element.childNodes.length; i++) {
			var node = element.childNodes[i];
			this.inlineWrap(node);
		}

	},
	
	
	bricolage_css_properties: ["azimuth",
	"background",
	"backgroundAttachment",
	"backgroundColor",
	"backgroundImage",
	"backgroundPosition",
	"backgroundRepeat",
	//"border",
	//"borderBottom",
	//"borderBottomColor",
	//"borderBottomStyle",
	//"borderBottomWidth",
	//"borderCollapse",
	//"borderColor",
	//"borderLeft",
	//"borderLeftColor",
	//"borderLeftStyle",
	//"borderLeftWidth",
	//"borderRight",
	//"borderRightColor",
	//"borderRightStyle",
	//"borderRightWidth",
	//"borderSpacing",
	//"borderStyle",
	//"borderTop",
	//"borderTopColor",
	//"borderTopStyle",
	//"borderTopWidth",
	//"borderWidth",
	//"bottom",
	"captionSide",
	"clear",
	"clip",
	"color",
	"content",
	"counterIncrement",
	"counterReset",
	"cssFloat",
	"cue",
	"cueAfter",
	"cueBefore",
	"cursor",
	"direction",
	//"display",
	"elevation",
	"emptyCells",
	//"float",
	"font",
	"fontFamily",
	"fontSize",
	"fontSizeAdjust",
	"fontStretch",
	"fontStyle",
	"fontVariant",
	"fontWeight",
	"height",
	"imeMode",
	//"left",
	"letterSpacing",
	"lineHeight",
	"listStyle",
	"listStyleImage",
	"listStylePosition",
	"listStyleType",
	//"margin",
	//"marginBottom",
	//"marginLeft",
	//"marginRight",
	//"marginTop",
	"markerOffset",
	"marks",
	"maxHeight",
	"maxWidth",
	"minHeight",
	"minWidth",
	"orphans",
	"outline",
	"outlineColor",
	"outlineStyle",
	"outlineWidth",
	"overflow",
	//"padding",
	//"paddingBottom",
	//"paddingLeft",
	//"paddingRight",
	//"paddingTop",
	"page",
	"pageBreakAfter",
	"pageBreakBefore",
	"pageBreakInside",
	"pause",
	"pauseAfter",
	"pauseBefore",
	"pitch",
	"pitchRange",
	//"position",
	"quotes",
	"richness",
	//"right",
	"size",
	"speak",
	"speakHeader",
	"speakNumeral",
	"speakPunctuation",
	"speechRate",
	"stress",
	"tableLayout",
	"textAlign",
	"textDecoration",
	"textIndent",
	"textShadow",
	"textTransform",
	//"top",
	"unicodeBidi",
	"verticalAlign",
	"visibility",
	"voiceFamily",
	"volume",
	"whiteSpace",
	"widows",
	"width",
	"wordSpacing",
	"zIndex"]
		
};



jQuery(function() {
	edu_stanford_bricolage.inlineWrap(document.body);
});



