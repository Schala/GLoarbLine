#include <Types.r>

resource 'ALRT' (128, purgeable) {
	{50, 30, 170, 410},
	128,
	{	/* array: 4 elements */
		/* [1] */
		OK, visible, sound1,
		/* [2] */
		OK, visible, sound1,
		/* [3] */
		OK, visible, sound1,
		/* [4] */
		OK, visible, sound1
	},
	alertPositionMainScreen
};

resource 'DITL' (128, purgeable) {
	{	/* array DITLarray: 2 elements */
		/* [1] */
		{88, 299, 108, 367},
		Button {
			enabled,
			"OK"
		},
		/* [2] */
		{21, 60, 83, 366},
		StaticText {
			disabled,
			"Hello World, this is a sample alert!"
		}
	}
};

