
#include "test.h"

class MarkupTest : public ::testing::Test
{
protected:
    static void SetUpTestSuite()
    {
        Test_Init();
    }

    static void TearDownTestSuite()
    {
        Test_Shutdown();
    }

    void SetUp() override
    {
        view = new TestWebView();
        loadTestData();
    }

    void TearDown() override
    {
        delete view;
    }

    Document* document() const
    {
        return view->mainframe()->document();
    }

    void loadTestData()
    {
        static const char* testHtml = R"(
        <!DOCTYPE html>
        <html>
        <head><meta charset="utf-8"><title>Markup Test</title></head>
        <body id="test-body">
          <div title="a&amp;b&lt;c&quot;d">Text with &amp; &lt; &gt;</div>
          <span style="color:red; font-weight:bold;">Styled span</span>
          <blockquote type="cite" class="gmail_quote">Mail blockquote</blockquote>
          <pre>Pre with
        newline</pre>
          <table><tr><td>Cell</td></tr></table>
          <ul><li>Item</li></ul>
          <p>Para1<br>Para2</p>
          <textarea>Tab\tSeparated</textarea>
          <!-- comment -->
        </body>
        </html>
        )";

        view->loadHtml(testHtml, "http://localhost/test");

        TestWebView::waitForDocumentComplete(view);
    }

    TestWebView* view;
};

TEST_F(MarkupTest, CreateFragmentFromMarkup)
{
    String markup = "<div title=\"a&amp;b&lt;c&quot;d\">x</div>";
    RefPtr<DocumentFragment> frag = createFragmentFromMarkup(document(), markup, "http://base.test/");
    ASSERT_TRUE(frag);
    EXPECT_EQ(frag->childNodes()->length(), 1u);
    Element* div = (Element*)frag->firstChild();
    EXPECT_EQ(div->getAttribute("title"), "a&b<c\"d");
}
