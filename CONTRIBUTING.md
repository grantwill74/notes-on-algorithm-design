# How to contribute
If you notice an error or have a suggestion for how to improve the wording or
graphics of a slide or other class material, please submit a pull request.

# Copyright of contributions and modifications
By opening a pull request, you assert implicitly that you own the copyright to
the material being submitted and hereby grant me a license to your contributions
under CC BY-SA 4.0. Note that this license is non-revokable.

If you do not own the copyright and are including material created by
others, please state this in the pull request so that we can determine
if the material can be included.

# Attribution of contributions
* **Default**, I'll list your github username in CREDITS.md.
* **If you want real-name or psuedonymous credit**:
    include a note letting me know what full name or psuedonym to use.
    I will add it to CREDITS.md.

> **Special FERPA note for my students:** 
> Because this activity is voluntary, ungraded,
> and takes place in a public GitHub repo, not inside our LMS gradebook, 
> these submissions
> aren’t part of your formal student record. You’re under no obligation to give
> your real name; but if you choose to, that’s your decision and I’ll gladly
> credit you.

# Slides
These lecture notes are mostly written in [Marp](https://marp.app/). Marp is
a technology for writing presentations in Markdown. Using Marp over
Powerpoint or LibreOffice Impress has a number of advantages:

1. Ease of writing: writing a new text slide is a bit faster,
    and doesn't require any complex layout.
2. Ease of contributing: slide decks can be remixed and merged using existing
    text workflows.
3. Accessibility: text flow and tab-order works the way you expect, aiding 
    screen narration. For other accessibility concerns, the raw text is
    actually human readable, unlike binary or XML presentation formats.

Please make slide contributions in MARP, and integrate them into the existing
marp documents (or createa a new one if suggesting an entirely new module).

Eventually, I intend to create a CI/CD pipeline that builds the PDFs from the
latest marp sourcecode, but until I do that, please manually include updated
PDFs in your PRs.

# Graphics
Please use SVG graphics whenever possible (e.g., for tree/graph diagrams). For
things that cannot use SVG graphics, raster graphics are okay. Please
always include alt-text for any graphical submissions. 

When using SVG graphics, please use mermaid, which is a
markdown-based language for describing diagrams, wherever possible. 

[Mermaid Live Editor](https://mermaid.live/edit) is useful for generating
SVG graphics with textual data.

If you use mermaid to generate graphics, please
also include the source code for the diagram in markdown comments
(using \<!-- ... --\>)  in the relevant slide. I wish it were possible to directly incorporate mermaid diagrams, but this doesn't seem like something that Marp will support.

