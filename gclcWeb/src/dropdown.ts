const closeAllDropdowns = (except?: Element) => {
  document.querySelectorAll(".dropdown.open").forEach((dropdown) => {
    if (dropdown !== except) {
      dropdown.classList.remove("open");
    }
  });
};

const setDropdownUI = () => {
  document.querySelectorAll<HTMLElement>(".dropdown").forEach((dropdown) => {
    const trigger = dropdown.querySelector<HTMLButtonElement>(":scope > button");
    const content = dropdown.querySelector<HTMLElement>(
      ":scope > .dropdownContent"
    );

    if (trigger == null || content == null) {
      return;
    }

    trigger.addEventListener("click", () => {
      closeAllDropdowns(dropdown);
      dropdown.classList.toggle("open");
    });

    content.addEventListener("click", (event) => {
      if (
        content.dataset.keepOpen === undefined &&
        (event.target as Element).closest("button") != null
      ) {
        dropdown.classList.remove("open");
      }
    });
  });

  document.addEventListener("click", (event) => {
    const clicked = (event.target as Element).closest(".dropdown");
    closeAllDropdowns(clicked ?? undefined);
  });

  document.addEventListener("keydown", (event) => {
    if (event.key === "Escape") {
      closeAllDropdowns();
    }
  });
};

export { setDropdownUI };
