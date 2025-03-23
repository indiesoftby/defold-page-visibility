var LibraryPageVisibilityJS = {
    $PageVisibilityJS: {
        stateToEnum: function (state) {
            switch (state) {
                case "visible": return 1;
                case "hidden": return 2;
                default: return -1;
            }
        },

        initialized: false,
        listener: null,
    },

    PageVisibilityJS_SetListener: function (callback) {
        if (!PageVisibilityJS.initialized) {
            PageVisibilityJS.initialized = true;
            document.addEventListener("visibilitychange", () => {
                if (PageVisibilityJS.listener) {
                    {{{ makeDynCall('v', 'PageVisibilityJS.listener') }}}();
                }
            });
        }

        PageVisibilityJS.listener = callback;
    },

    PageVisibilityJS_GetState: function () {
        return PageVisibilityJS.stateToEnum(document.visibilityState);
    },
};

autoAddDeps(LibraryPageVisibilityJS, "$PageVisibilityJS");
mergeInto(LibraryManager.library, LibraryPageVisibilityJS);