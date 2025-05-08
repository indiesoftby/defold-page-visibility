# Page Visibility API for Defold (HTML5)

This extension for [the Defold game engine](https://defold.com/) provides a way to detect when the page is visible or hidden using [Page Visibility API](https://developer.mozilla.org/en-US/docs/Web/API/Page_Visibility_API).

Typically, this is used in HTML5 games to detect page visibility and pause/resume audio and other systems when the page is hidden.

## Why use Page Visibility API instead of Window Focus/Blur events?

While Defold provides `window.set_listener` to detect focus and blur events, using Page Visibility API offers a more appropriate behavior for games, especially when embedded in iframes:

- **Window Focus/Blur** (in Defold: `window.WINDOW_EVENT_FOCUS_LOST`/`window.WINDOW_EVENT_FOCUS_GAINED`) events will trigger even when your game is running in an iframe and the user clicks on other elements on the same page. This can lead to unintended pausing of your game when the player is still viewing it.

- **Page Visibility API** only triggers when the user actually switches tabs or minimizes the browser window. This creates a more natural experience for players, as the game only pauses when they're truly not viewing it.

This difference is particularly important for games embedded on game portals or websites with multiple interactive elements.

## Setup

First add this extension as a dependency to your `game.project`:

    https://github.com/indiesoftby/defold-page-visibility/archive/main.zip

Then you can set a listener for the page visibility change event:

```lua
function init(self)
    if not page_visibility then
        print("Page visibility extension is only available on HTML5")
        return
    end

    print("Current page visibility state: " .. (page_visibility.state() == page_visibility.STATE_VISIBLE and "visible" or "hidden"))

    page_visibility.set_listener(function()
        local state = page_visibility.state()
        if state == page_visibility.STATE_VISIBLE then
            print("Page visibility changed: visible")
        elseif state == page_visibility.STATE_HIDDEN then
            print("Page visibility changed: hidden")
        end
    end)
end
```

## API

### `page_visibility.set_listener(callback)`
Sets or removes the listener for the page visibility change event.

**Parameters:**
- `callback` (function, optional): The callback to be called when the page visibility changes. Pass nil to remove the listener.

### `page_visibility.state()`
Returns the current state of the page visibility.

**Returns:**
- `number`: The current state of the page visibility (one of the constants below).

### `page_visibility.STATE_VISIBLE`
Constant indicating that the page is visible.

### `page_visibility.STATE_HIDDEN`
Constant indicating that the page is hidden.

## License

This project is licensed under the MIT License. See the LICENSE file for details.
