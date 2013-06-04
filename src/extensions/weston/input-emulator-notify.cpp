/*
 * Copyright © 2013 Intel Corporation
 *
 * Permission to use, copy, modify, distribute, and sell this software and
 * its documentation for any purpose is hereby granted without fee, provided
 * that the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name of the copyright holders not be used in
 * advertising or publicity pertaining to distribution of the software
 * without specific, written prior permission.  The copyright holders make
 * no representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 *
 * THE COPYRIGHT HOLDERS DISCLAIM ALL WARRANTIES WITH REGARD TO THIS
 * SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS, IN NO EVENT SHALL THE COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER
 * RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF
 * CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <cassert>
#include <linux/input.h>

#include "common/util.h"
#include "weston-wfits.h"
#include "input-emulator-notify.h"

namespace wfits {
namespace weston {

/*static*/ bool InputEmulatorNotify::registered_ = InputEmulatorFactory::registerEmulator("notify", Create<InputEmulatorNotify>());

InputEmulatorNotify::InputEmulatorNotify()
	: InputEmulator::InputEmulator()
{
	return;
}

/*virtual*/
InputEmulatorNotify::~InputEmulatorNotify()
{
	weston_log("weston-wfits: %s\n", BOOST_CURRENT_FUNCTION);
}

/*virtual*/
void InputEmulatorNotify::movePointer(const int32_t x, const int32_t y) const
{
	struct weston_seat *seat(Globals::seat());
	wl_fixed_t cx, cy;

	Globals::compositor()->focus = 1;

	Globals::pointerXY(&cx, &cy);

	notify_motion(
		seat, 100,
		wl_fixed_from_int(x) - cx,
		wl_fixed_from_int(y) - cy
	);
}

/*virtual*/
void InputEmulatorNotify::keySend(const uint32_t key, const uint32_t state) const
{
	struct weston_seat *seat(Globals::seat());

	Globals::compositor()->focus = 1;

	if (key == BTN_LEFT or key == BTN_MIDDLE or key == BTN_RIGHT) {
		notify_button(
			seat, 100, key,
			static_cast<wl_pointer_button_state>(state)
		);
	} else {
		assert(key >= 0 and key < 255);
		notify_key(
			seat, 100, key,
			static_cast<wl_keyboard_key_state>(state),
			STATE_UPDATE_AUTOMATIC
		);
	}
}

} // namespace weston
} // namespace wfits