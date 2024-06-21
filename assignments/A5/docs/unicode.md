# Part 3 Unicode

### 1. Who founded the Consortium? Who is represented among the current members, and how might that affect decisions being made for Unicode?

The Unicode Consortium began as a collaborative effort between engineers Joe Becker, Lee Collins and Mark Davis in 1987.
It was not formally incorporated until 1991, by which point several major computing companies had joined the project.
Its current members include many of the world's largest and most influential technology companies, such as Google, Microsoft, and Amazon.
Naturally, one might imagine that these members would want any major decisions regarding Unicode to be in their best interest.
Many of these companies are based in the United States, meaning that the backbone of virtually all online communication is managed by people who likely only speak English.
What's more, the Unicode standard covers not only text characters but also emoji.
The choice of which symbols get added to the list is highly dependent on the culture of those in charge.
There are a considerable number of individual members, however, who are hopefully able to better represent the diversity of communication in our world.

[Unicode History](https://www.unicode.org/history/summary.html)
[Unicode Members](https://home.unicode.org/membership/members/)

### 2. Find a language that is not yet in Unicode, has only recently been supported (e.g., in the last 10 years), or is/was missing important characters. What is/was missing, and how does this affect the users of the language?

The Wancho script was only added to Unicode in 2019.
The Wancho language is used in some parts of India, though most speakers write using either Devanagari or Latin characters.
The proposal notes that "The Wancho script is taught today in schools."
Before this script was added, children in these regions wouldn't have been able to use technology with the characters they were familiar with.
With the prevalence of technology today, this could have contributed to a decline in native Wancho speakers, or at least some loss of the language's identity if speakers were forced to use a different alphabet.

[Wancho Proposal](https://www.unicode.org/L2/L2017/17067r-n4787r-wancho.pdf)

### 3. For this question, you will need to work with a classmate! Make sure to coordinate so that you outline opposing positions for Part A (e.g. if you outline the ‘for’ position, your partner defends the ‘against’ position). Then, come together and discuss Part B!




#### Step A: Outline either the position for or against Han Unification

For: Ultimately, Unicode is limited by the same limitations as all modern technology: ones and zeroes. This means that some compromises have to be made with characters that look and function similarly between languages. An inordinate proportion of the Unicode character space would need to be dedicated to languages with large numbers of glyphs (like Chinese) otherwise. We might then need five-byte characters, which would add another layer of complexity to an already monstrous standard.

Against: While the Han characters may share the same roots across languages, they may not be used in the same way in every context. Languages naturally evolve over time, not only in their phonology and grammar but also in how they are written. In some cases, a character may be drawn slightly differently depending on the language. If these characters are unified, people will probably still be able to understand what they're reading, but doing so will still strip a noticeable amount of identity from the affected languages.


#### Step B
### A bullet-point summary of your discussion for Step B


#### Describe the tradeoff that Unicode has made with this decision 

By and large, Unicode went ahead with Han Unification, which has helped them reduce the amount of space needed for Han characters. However, they have been willing to add regional variants of the affected characters when the glyphs are considerably different from one another.